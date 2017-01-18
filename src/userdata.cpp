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

#include "userdata.h"
#include "roommodel.h"
#include "ddpclient.h"

UserData *UserData::m_self = 0;

QString UserData::authToken() const
{
    return m_authToken;
}

QString UserData::userName() const
{
    return m_userName;
}

QString UserData::password() const
{
    return m_password;
}

void UserData::setAuthToken(const QString& token)
{
    qDebug() << "Setting token to" << token;
    QSettings s;
    m_authToken = token;
    s.setValue("authToken", token);
}

void UserData::setPassword(const QString& password)
{
    m_password = password;
}

void UserData::setUserName(const QString& username)
{
    m_userName = username;
    QSettings s;
    s.setValue("username", username);
    emit userNameChanged();
}

RoomModel * UserData::roomModel()
{
    if (!m_roomModel) {
        qDebug() << "creating new RoomModel";
        m_roomModel = new RoomModel(this);
        qDebug() << m_roomModel;
//         m_roomModel->reset();
    }
    return m_roomModel;
}

DDPClient * UserData::ddp()
{
    if (!m_ddp) {
        m_ddp = new DDPClient(serverURL());
        connect(m_ddp, &DDPClient::loginStatusChanged, this, &UserData::loginStatusChanged);
//         connect(m_ddp, &DDPClient::loginStatusChanged, this, [=](){qDebug() << "Signal received";});
    }
    return m_ddp;
}

void UserData::sendMessage(const QString &roomID, const QString &message)
{
    QString json = "{\"rid\": \"%1\", \"msg\": \"%2\"}";
    json = json.arg(roomID, message);
    ddp()->method("sendMessage", QJsonDocument::fromJson(json.toUtf8()));
}

MessageModel * UserData::getModelForRoom(const QString& roomID)
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

QString UserData::serverURL() const
{
    return m_serverURL;
}

void UserData::setServerURL(const QString& serverURL)
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

DDPClient::LoginStatus UserData::loginStatus()
{
    if (m_ddp) {
        return ddp()->loginStatus();
    } else {
        return DDPClient::LoggedOut;
    }
}

void UserData::tryLogin()
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

void UserData::logOut()
{
    setAuthToken(QString());
    setPassword(QString());
//     m_ddp->logOut();
    foreach (const QString key, m_messageModels.keys()) {
        MessageModel *m = m_messageModels.take(key);
        delete m;
    }
    delete m_ddp;
    m_ddp = 0;
    emit loginStatusChanged();
    
    m_roomModel->clear();
//     m_roomModel->reset();
    // RoomModel -> reset();
}

QString UserData::cacheBasePath() const
{
    if (m_serverURL.isEmpty()) {
        return QString();
    }
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation)+'/'+m_serverURL;
}

// QString UserData::activeRoom() const
// {
//     return m_activeRoom;
// }
// void UserData::setActiveRoom(const QString& activeRoom)
// {
//     m_activeRoom = activeRoom;
// //     roomModel()->setActiveRoom(activeRoom);
//     emit activeRoomChanged();
// }
RoomWrapper * UserData::getRoom(const QString& roomID)
{
    return roomModel()->findRoom(roomID);
}


UserData::UserData(QObject* parent)
 : QObject(parent),
 m_ddp(0),
 m_roomModel(0)
{
    QSettings s;
    m_serverURL = s.value("serverURL", "demo.rocket.chat").toString();
    m_userName = s.value("username").toString();
    m_authToken = s.value("authToken").toString();
//     roomModel()->reset();
}

UserData * UserData::self() 
{
    if (!m_self) {
        m_self = new UserData;
        m_self->ddp(); // Create DDP object so we try to connect at startup
        m_self->roomModel()->reset();
//         m_self->getModelForRoom("GENERAL");
    }
    return m_self;
}

