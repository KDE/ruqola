/*
 * <one line to give the program's name and a brief idea of what it does.>
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
#include "ddpclient.h"
#include "notification.h"
#include <QFileDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>

Ruqola *Ruqola::m_self = 0;

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

void Ruqola::setAuthToken(const QString& token)
{
    qDebug() << "Setting token to" << token;
    QSettings s;
    m_authToken = token;
    s.setValue("authToken", token);
}

void Ruqola::setPassword(const QString& password)
{
    m_password = password;
}

void Ruqola::setUserName(const QString& username)
{
    m_userName = username;
    QSettings s;
    s.setValue("username", username);
    emit userNameChanged();
}

void Ruqola::setUserID(const QString& userID)
{
    m_userName = userID;
    QSettings s;
    s.setValue("userID", userID);
    emit userIDChanged();
}

RoomModel * Ruqola::roomModel()
{
    if (!m_roomModel) {
        qDebug() << "creating new RoomModel";
        m_roomModel = new RoomModel(this);
        qDebug() << m_roomModel;
    }
    return m_roomModel;
}

DDPClient * Ruqola::ddp()
{
    if (!m_ddp) {
        m_ddp = new DDPClient(serverURL());
        connect(m_ddp, &DDPClient::loginStatusChanged, this, &Ruqola::loginStatusChanged);
//         connect(m_ddp, &DDPClient::loginStatusChanged, this, [=](){qDebug() << "Signal received";});
    }
    return m_ddp;
}

MessageQueue * Ruqola::messageQueue()
{
    if (!m_messageQueue) {
        m_messageQueue = new MessageQueue();
        // retry to send any unsent messages
        Ruqola::self()->messageQueue()->retry();
//        connect(m_messageQueue, &DDPClient::loginStatusChanged, this, &MessageQueue::loginStatusChanged);
    }
    return m_messageQueue;
}


Notification * Ruqola::notification()
{
    if (m_notification == NULL) {
        m_notification = new Notification();
        m_notification->show();
    }
    return m_notification;
}

void Ruqola::attachmentButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR,
                                              "Select one or more files to open",
                                              QDir::homePath(),
                                              "Images (*.png *.jpeg *.jpg)");

    qDebug() << "Selected Image " << fileName;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Cannot open the selected file";
        return;
    }
    const QString message = QString::fromLatin1(file.readAll().toBase64());
    const QString roomID("3cGRyFLWgnPL7B79n"); //hard code roomID for now
    const QString type("image");
    sendMessage(roomID, message, type);
}

void Ruqola::sendMessage(const QString &roomID, const QString &message, const QString &type)
{
    QJsonObject json;
    json["rid"] = roomID;
    json["msg"] = message;
    json["type"] = type;

    ddp()->method("sendMessage", QJsonDocument(json));
}

MessageModel * Ruqola::getModelForRoom(const QString& roomID)
{
    if (m_messageModels.contains(roomID)) {
//         qDebug() << "Returning old model for " << roomID;
        return m_messageModels.value(roomID);
    } else {
//         qDebug() << "Creating a new model";
        m_messageModels[roomID] = new MessageModel(roomID, this);

        return m_messageModels[roomID];
    }
}

QString Ruqola::serverURL() const
{
    return m_serverURL;
}

void Ruqola::setServerURL(const QString& serverURL)
{
    if (m_serverURL == serverURL) {
        return;
    }

    QSettings s;
    s.setValue("serverURL", serverURL);
    m_serverURL = serverURL;
//     m_roomModel->reset();
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
    qDebug() << "Attempting login" << userName() << "on" << serverURL();

    // Reset model views
    foreach (const QString key, m_messageModels.keys()) {
        MessageModel *m = m_messageModels.take(key);
        delete m;
    }
    delete m_ddp;
    m_ddp = 0;

    // In the meantime, load cache...
    m_roomModel->reset();

    // This creates a new ddp() object.
    // DDP will automatically try to connect and login.
    ddp();
}

void Ruqola::logOut()
{
    setAuthToken(QString());
    setPassword(QString());
    foreach (const QString key, m_messageModels.keys()) {
        MessageModel *m = m_messageModels.take(key);
        delete m;
    }
    delete m_ddp;
    m_ddp = 0;
    emit loginStatusChanged();

    m_roomModel->clear();
}

QString Ruqola::cacheBasePath() const
{
    if (m_serverURL.isEmpty()) {
        return QString();
    }
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+'/'+m_serverURL;
}

// QString Ruqola::activeRoom() const
// {
//     return m_activeRoom;
// }
// void Ruqola::setActiveRoom(const QString& activeRoom)
// {
//     m_activeRoom = activeRoom;
// //     roomModel()->setActiveRoom(activeRoom);
//     emit activeRoomChanged();
// }

RoomWrapper * Ruqola::getRoom(const QString& roomID)
{
    return roomModel()->findRoom(roomID);
}


Ruqola::Ruqola(QObject* parent): QObject(parent), m_ddp(0), m_messageQueue(0), m_roomModel(0), m_notification(0)
{
    QSettings s;
    m_serverURL = s.value("serverURL", "demo.rocket.chat").toString();
    m_userName = s.value("username").toString();
    m_userID = s.value("userID").toString();
    m_authToken = s.value("authToken").toString();
}

Ruqola * Ruqola::self()
{
    if (!m_self) {
        m_self = new Ruqola;

        // Create DDP object so we try to connect at startup
        m_self->ddp();

        // Clear rooms data and refill it with data in the cache, if there is
        m_self->roomModel()->reset();

        // Create systray to show notifications
        m_self->notification();

        //Initialize the messageQueue object
        m_self->messageQueue();
    }
    return m_self;
}


