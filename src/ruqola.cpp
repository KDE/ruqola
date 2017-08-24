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
#include "ddpapi/ddpclient.h"
#include "notification.h"
#include "messagequeue.h"
#include "ruqola_debug.h"
#include "roomfilterproxymodel.h"
#include "restapi/restapirequest.h"
#include <QFileDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>

Ruqola::Ruqola(QObject *parent)
    : QObject(parent)
    , mDdp(nullptr)
    , m_messageQueue(nullptr)
    , mRoomModel(nullptr)
    , mRoomFilterProxyModel(nullptr)
    , mNotification(nullptr)
    , mTypingNotification(nullptr)
{
    mRoomModel = new RoomModel(this);
    mRoomFilterProxyModel = new RoomFilterProxyModel(this);
    mRoomFilterProxyModel->setSourceModel(mRoomModel);


    mTypingNotification = new TypingNotification(this);
    connect(mTypingNotification, &TypingNotification::informTypingStatus, this, &Ruqola::slotInformTypingStatus);
    QSettings s;
    mServerURL = s.value(QStringLiteral("serverURL"), QStringLiteral("demo.rocket.chat")).toString();
    mUserName = s.value(QStringLiteral("username")).toString();
    mUserID = s.value(QStringLiteral("userID")).toString();
    mAuthToken = s.value(QStringLiteral("authToken")).toString();
}

RoomFilterProxyModel *Ruqola::roomFilterProxyModel() const
{
    return mRoomFilterProxyModel;
}

Ruqola *Ruqola::self()
{
    static Ruqola *s_self = nullptr;
    if (!s_self) {
        s_self = new Ruqola;

        // Create DDP object so we try to connect at startup
        s_self->ddp();

        // Clear rooms data and refill it with data in the cache, if there is
        s_self->roomModel()->reset();

        // Create systray to show notifications on Desktop
#if !defined(Q_OS_ANDROID) || !defined(Q_OS_IOS)
        s_self->notification();
#endif

        //Initialize the messageQueue object
        s_self->messageQueue();
    }
    return s_self;
}

QString Ruqola::authToken() const
{
    return mAuthToken;
}

QString Ruqola::userName() const
{
    return mUserName;
}

QString Ruqola::userID() const
{
    return mUserID;
}

QString Ruqola::password() const
{
    return m_password;
}

void Ruqola::setAuthToken(const QString &token)
{
    if (mAuthToken != token) {
        qCDebug(RUQOLA_LOG) << "Setting token to" << token;
        QSettings s;
        mAuthToken = token;
        s.setValue(QStringLiteral("authToken"), token);
    }
}

void Ruqola::setPassword(const QString &password)
{
    m_password = password;
}

void Ruqola::setUserName(const QString &username)
{
    if (mUserName != username) {
        mUserName = username;
        QSettings s;
        s.setValue(QStringLiteral("username"), username);
        Q_EMIT userNameChanged();
    }
}

void Ruqola::setUserID(const QString &userID)
{
    //Don't use if( m_userID != userID) as we need to Q_EMIT userIDChanged
    mUserID = userID;
    QSettings s;
    s.setValue(QStringLiteral("userID"), userID);
    Q_EMIT userIDChanged();
}

RoomModel *Ruqola::roomModel()
{
    return mRoomModel;
}

RestApiRequest *Ruqola::restapi()
{
    if (!mRestApi) {
        mRestApi = new RestApiRequest(this);
        mRestApi->setServerUrl(serverURL());
    }
    return mRestApi;
}

DDPClient *Ruqola::ddp()
{
    if (!mDdp) {
        mDdp = new DDPClient();
        mDdp->setServerUrl(serverURL());
        mDdp->start();
        connect(mDdp, &DDPClient::loginStatusChanged, this, &Ruqola::loginStatusChanged);
    }
    return mDdp;
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
    if (!mNotification) {
        mNotification = new Notification();
        mNotification->show();
    }
    return mNotification;
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
    return mServerURL;
}

void Ruqola::setServerURL(const QString &serverURL)
{
    if (mServerURL == serverURL) {
        return;
    }

    QSettings s;
    s.setValue(QStringLiteral("serverURL"), serverURL);
    mServerURL = serverURL;
    Q_EMIT serverURLChanged();
}

DDPClient::LoginStatus Ruqola::loginStatus()
{
    if (mDdp) {
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
    delete mDdp;
    mDdp = nullptr;

    // This creates a new ddp() object.
    // DDP will automatically try to connect and login.
    ddp();
    restapi();
    restapi()->setPassword(password());
    restapi()->login();

    // In the meantime, load cache...
    //if(Ruqola::self()->ddp()->isConnected() && Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
    mRoomModel->reset();
    //}
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

    mRoomModel->clear();

    QJsonObject user;
    user[QStringLiteral("username")] = Ruqola::self()->userName();
    QJsonObject json;
    json[QStringLiteral("user")] = user;
    Ruqola::self()->ddp()->method(QStringLiteral("logout"), QJsonDocument(json));

    delete mDdp;
    mDdp = nullptr;
    Q_EMIT loginStatusChanged();
    qCDebug(RUQOLA_LOG) << "Successfully logged out!";
}

QString Ruqola::cacheBasePath() const
{
    if (mServerURL.isEmpty()) {
        return QString();
    }

    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+QLatin1Char('/')+mServerURL;
}

RoomWrapper *Ruqola::getRoom(const QString &roomID)
{
    return roomModel()->findRoom(roomID);
}

void Ruqola::slotInformTypingStatus(const QString &room, bool typing)
{
    ddp()->informTypingStatus(room, typing, mUserName);
}
