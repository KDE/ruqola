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

#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>
#include "libruqolacore_export.h"
#include <QQmlEngine>
#include <QJSEngine>
#include "ddpapi/ddpclient.h"
#include "roommodel.h"
#include "messagemodel.h"
#include "notification.h"

class TypingNotification;
class UsersModel;
class RestApiRequest;
class RocketChatAccount;
class UsersForRoomModel;
class LIBRUQOLACORE_EXPORT Ruqola : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString userID READ userID WRITE setUserID NOTIFY userIDChanged)
    Q_PROPERTY(QString serverURL READ serverURL WRITE setServerURL NOTIFY serverURLChanged)
    Q_PROPERTY(QString password WRITE setPassword)
    Q_PROPERTY(DDPClient::LoginStatus loginStatus READ loginStatus NOTIFY loginStatusChanged)

public:

    /**
    * @brief Singleton provider
    *
    * @return Returns the singleton object m_self
    */
    static Ruqola *self();

    void setUserName(const QString &username);
    QString userName() const;

    void setUserID(const QString &userID);
    QString userID() const;

    void setPassword(const QString &password);
    QString password() const;

    void setAuthToken(const QString &token);
    QString authToken() const;

    DDPClient::LoginStatus loginStatus();

    QString serverURL() const;
    void setServerURL(const QString &serverURL);

    DDPClient *ddp() const;
    Notification *notification();

    Q_INVOKABLE RoomModel *roomModel() const;

    /**
    * @brief Constructs a Json with @param roomID and @param message and @param type, then calls DDPClient's method to send text message over the network
    */
    Q_INVOKABLE void sendMessage(const QString &roomID, const QString &message, const QString &type);

    Q_INVOKABLE void leaveRoom(const QString &roomID);
    Q_INVOKABLE void hideRoom(const QString &roomID);

    /**
    * @brief Returns a model for room with ID @param roomID
    *
    * @return MessageModel Pointer, model for room
    */
    Q_INVOKABLE MessageModel *getMessageModelForRoom(const QString &roomID);

    /**
    * @brief Reset models, load cache and call DDPClient's object to automatically try to connect and log in via username and password
    */
    Q_INVOKABLE void tryLogin();

    /**
    * @brief Clear models, stores cache and logs out the user
    */
    Q_INVOKABLE void logOut();
    /**
    * @brief Finds room with @param roomID
    *
    * @return RoomWrapper Pointer, The room model for @param roomID
    */
    Q_INVOKABLE RoomWrapper *getRoom(const QString &roomID);

    Q_INVOKABLE void textEditing(const QString &roomId, const QString &str);
    Q_INVOKABLE QString getUserCurrentMessage(const QString &roomId);
    Q_INVOKABLE void setUserCurrentMessage(const QString &message, const QString &roomId);


    RestApiRequest *restapi() const;
    Q_INVOKABLE UsersForRoomModel *getUsersForRoomModel(const QString &roomId);
    Q_INVOKABLE UsersModel *usersModel() const;

    Q_INVOKABLE RocketChatAccount *rocketChatAccount() const;

Q_SIGNALS:
    void userNameChanged();
    void userIDChanged();
    void serverURLChanged();
    void loginStatusChanged();

private:
    void sendNotification(const QString &title, const QString &message);
    void slotInformTypingStatus(const QString &room, bool typing);
    explicit Ruqola(QObject *parent = nullptr);
    RocketChatAccount *mRocketChatAccount;
    Notification *mNotification = nullptr;
};

inline static QObject *ruqola_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Ruqola *userData = Ruqola::self();
    return userData;
}

#endif // USERDATA_H
