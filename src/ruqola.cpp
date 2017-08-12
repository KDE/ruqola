/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ruqola.h"
#include "roommodel.h"
#include "typingnotification.h"
#include "ddpclient.h"
#include "notification.h"
#include "messagequeue.h"
#include "ruqola_debug.h"
#include "roomfilterproxymodel.h"
#include <QFileDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

Ruqola *Ruqola::m_self = nullptr;


Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
    , m_ddp(nullptr)
    , m_messageQueue(nullptr)
    , m_roomModel(nullptr)
    , mRoomFilterProxyModel(nullptr)
    , m_notification(nullptr)
    , m_authentication(nullptr)
    , mTypingNotification(nullptr)
{
    m_roomModel = new RoomModel(this);
    mRoomFilterProxyModel = new RoomFilterProxyModel(this);
    mRoomFilterProxyModel->setSourceModel(m_roomModel);

    mTypingNotification = new TypingNotification(this);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &Ruqola::slotInformTypingStatus);
    QSettings s;
    m_serverURL = s.value(QStringLiteral("serverURL"), QStringLiteral("demo.rocket.chat")).toString();
    m_userName = s.value(QStringLiteral("username")).toString();
    m_userID = s.value(QStringLiteral("userID")).toString();
    m_authToken = s.value(QStringLiteral("authToken")).toString();
}

RoomFilterProxyModel *Ruqola::roomFilterProxyModel() const
{
    return mRoomFilterProxyModel;
}

Ruqola *Ruqola::self()
{
    if (!m_self) {
        m_self = new Ruqola;

        // Create DDP object so we try to connect at startup
        m_self->ddp();

        // Clear rooms data and refill it with data in the cache, if there is
        m_self->roomModel()->reset();

        // Create systray to show notifications on Desktop
#if !defined(Q_OS_ANDROID) || !defined(Q_OS_IOS)
        m_self->notification();
#endif

        //Initialize the messageQueue object
        m_self->messageQueue();

        //Initialize the OAuth object
        m_self->authentication();
    }
    return m_self;
}


QString Ruqola::authToken() const
{
    return m_authToken;
}

QString Ruqola::userName() const
{
    return m_userName;
}

QString Ruqola::userID() const
{
    return m_userID;
}

QString Ruqola::password() const
{
    return m_password;
}

void Ruqola::setAuthToken(const QString &token)
{
    if (m_authToken != token) {
        qCDebug(RUQOLA_LOG) << "Setting token to" << token;
        QSettings s;
        m_authToken = token;
        s.setValue(QStringLiteral("authToken"), token);
    }
}

void Ruqola::setPassword(const QString &password)
{
    m_password = password;
}

void Ruqola::setUserName(const QString &username)
{
    if (m_userName != username) {
        m_userName = username;
        QSettings s;
        s.setValue(QStringLiteral("username"), username);
        emit userNameChanged();
    }
}

void Ruqola::setUserID(const QString &userID)
{
    //Don't use if( m_userID != userID) as we need to emit userIDChanged
    m_userID = userID;
    QSettings s;
    s.setValue(QStringLiteral("userID"), userID);
    emit userIDChanged();
}

RoomModel *Ruqola::roomModel()
{
    return m_roomModel;
}

DDPClient *Ruqola::ddp()
{
    if (!m_ddp) {
        m_ddp = new DDPClient(serverURL());
        connect(m_ddp, &DDPClient::loginStatusChanged, this, &Ruqola::loginStatusChanged);
    }
    return m_ddp;
}

MessageQueue *Ruqola::messageQueue()
{
    if (!m_messageQueue) {
        m_messageQueue = new MessageQueue();
        // retry to send any unsent messages
        Ruqola::self()->messageQueue()->processQueue();
    }
    return m_messageQueue;
}

Notification *Ruqola::notification()
{
    if (!m_notification) {
        m_notification = new Notification();
        m_notification->show();
    }
    return m_notification;
}

Authentication *Ruqola::authentication()
{
    if (!m_authentication) {
        m_authentication = new Authentication();
    }
    return m_authentication;
}

void Ruqola::attachmentButtonClicked(const QString &roomId)
{
    const QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                          tr("Select one or more files to open"),
                                                          QDir::homePath(),
                                                          tr("Images (*.png *.jpeg *.jpg)"));
    if (fileName.isEmpty()) {
        return;
    }

    qCDebug(RUQOLA_LOG) << "Selected Image " << fileName;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qCDebug(RUQOLA_LOG) << "Cannot open the selected file" << fileName;
        return;
    }
    const QString message = QString::fromLatin1(file.readAll().toBase64());
    const QString roomID(roomId);
    const QString type(QStringLiteral("image"));
    sendMessage(roomID, message, type);
}

void Ruqola::textEditing(const QString &roomId, const QString &str)
{
    mTypingNotification->setText(roomId, str);
}

void Ruqola::sendMessage(const QString &roomID, const QString &message, const QString &type)
{
    QJsonObject json;
    json[QStringLiteral("rid")] = roomID;
    json[QStringLiteral("msg")] = message;
    json[QStringLiteral("type")] = type;

    ddp()->method(QStringLiteral("sendMessage"), QJsonDocument(json), DDPClient::Persistent);
}

void Ruqola::leaveRoom(const QString &roomID)
{
    ddp()->leaveRoom(roomID);
}

void Ruqola::hideRoom(const QString &roomID)
{
    ddp()->hideRoom(roomID);
}

MessageModel *Ruqola::getMessageModelForRoom(const QString &roomID)
{
    if (MessageModel *model = m_messageModels.value(roomID)) {
        return model;
    } else {
        m_messageModels[roomID] = new MessageModel(roomID, this);
        return m_messageModels[roomID];
    }
}

QString Ruqola::serverURL() const
{
    return m_serverURL;
}

void Ruqola::setServerURL(const QString &serverURL)
{
    if (m_serverURL == serverURL) {
        return;
    }

    QSettings s;
    s.setValue(QStringLiteral("serverURL"), serverURL);
    m_serverURL = serverURL;
    emit serverURLChanged();
}

DDPClient::LoginStatus Ruqola::loginStatus()
{
    if (m_ddp) {
        return ddp()->loginStatus();
    } else {
        return DDPClient::LoggedOut;
    }
}

void Ruqola::tryLogin()
{
    qCDebug(RUQOLA_LOG) << "Attempting login" << userName() << "on" << serverURL();

    // Reset model views
    foreach (const QString &key, m_messageModels.keys()) {
        MessageModel *m = m_messageModels.take(key);
        delete m;
    }
    delete m_ddp;
    m_ddp = nullptr;

    // This creates a new ddp() object.
    // DDP will automatically try to connect and login.
    ddp();

    // In the meantime, load cache...
    //if(Ruqola::self()->ddp()->isConnected() && Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
    m_roomModel->reset();
    //}
}

void Ruqola::tryOAuthLogin()
{
    // Reset model views
    foreach (const QString &key, m_messageModels.keys()) {
        MessageModel *m = m_messageModels.take(key);
        delete m;
    }
    delete m_ddp;
    m_ddp = nullptr;

    // This creates a new ddp() object.
    // DDP will automatically try to connect and login.
    ddp();

    m_roomModel->reset();

    if (Ruqola::self()->ddp()->isConnected()) {
        m_authentication->OAuthLogin();
    }
}

void Ruqola::logOut()
{
    QSettings s;
    s.setValue(QStringLiteral("authToken"), QString());
    setAuthToken(QString());
    setPassword(QString());

    foreach (const QString &key, m_messageModels.keys()) {
        MessageModel *m = m_messageModels.take(key);
        delete m;
    }

    m_roomModel->clear();

    QJsonObject user;
    user[QStringLiteral("username")] = Ruqola::self()->userName();
    QJsonObject json;
    json[QStringLiteral("user")] = user;
    Ruqola::self()->ddp()->method(QStringLiteral("logout"), QJsonDocument(json));

    delete m_ddp;
    m_ddp = nullptr;
    emit loginStatusChanged();
    qCDebug(RUQOLA_LOG) << "Successfully logged out!";
}

QString Ruqola::cacheBasePath() const
{
    if (m_serverURL.isEmpty()) {
        return QString();
    }

    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+QLatin1Char('/')+m_serverURL;
}

RoomWrapper *Ruqola::getRoom(const QString &roomID)
{
    return roomModel()->findRoom(roomID);
}

void Ruqola::slotInformTypingStatus(const QString &room, bool typing)
{
    ddp()->informTypingStatus(room, typing, m_userName);
}
