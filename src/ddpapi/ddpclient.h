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

#ifndef DDPCLIENT_H
#define DDPCLIENT_H

#include "rocketchatmessage.h"
#include "libruqolacore_export.h"
#include <functional>
#include <QQueue>

class QJsonObject;
class QJsonDocument;
class RocketChatMessage;
class AbstractWebSocket;
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT DDPClient : public QObject
{
    Q_OBJECT
public:
    enum LoginStatus {
        NotConnected,
        LoggingIn,
        LoggedIn,
        LoginFailed,
        LoggedOut
    };
    Q_ENUM(LoginStatus)

    enum MessageType {
        Persistent,
        Ephemeral
    };

    explicit DDPClient(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~DDPClient();

    /**
    * @brief Call a method with name @param method and parameters @param params and @param messageType with an empty callback
    *
    * @param method The name of the method to call Rocket.Chat API for
    * @param params The parameters
    * @param messageType The type of message
    * @return unsigned int, the ID of the called method
    */
    quint64 method(const QString &method, const QJsonDocument &params, DDPClient::MessageType messageType = DDPClient::Ephemeral);

    /**
    * @brief Send message over network
    *
    * @param method The name of the method to call Rocket.Chat API for
    * @param params The parameters
    * @param callback The pointer to callback function
    * @param messageType The type of message
    * @return unsigned int, the ID of the called method
    */
    quint64 method(const QString &method, const QJsonDocument &params, std::function<void(QJsonObject, RocketChatAccount *)> callback, DDPClient::MessageType messageType = DDPClient::Ephemeral);

    quint64 method(const RocketChatMessage::RocketChatMessageResult &result, std::function<void(QJsonObject, RocketChatAccount *)> callback, DDPClient::MessageType messageType = DDPClient::Ephemeral);

    /**
    * @brief Subscribes to a collection with name @param collection and parameters @param params
    *
    * @param collection The name of the collection
    * @param params The parameters
    */
    void subscribe(const QString &collection, const QJsonArray &params);

    /**
    * @brief Calls method to log in the user with valid username and password
    */
    Q_INVOKABLE void login();

    /**
    * @brief Closes the websocket connection
    */
    void logOut();

    /**
    * @brief Check whether websocket is connected at url
    *
    * @return true if connected, else false
    */
    Q_REQUIRED_RESULT bool isConnected() const;

    /**
    * @brief Check whether user is logged in
    *
    * @return true if user is logged in, else false
    */
    Q_REQUIRED_RESULT bool isLoggedIn() const;

    /**
    * @brief Reconnects the websocket to new url
    */
    void onServerURLChange();

    /**
    * @brief Returns the queue used to cache unsent messages
    *
    *@return QQueue<QPair<QString,QJsonDocument>>, The m_messageQueue object
    */
    Q_REQUIRED_RESULT QQueue<QPair<QString, QJsonDocument> > messageQueue() const;

    /**
    * @brief Returns standard cache path
    *
    *@def QString path
    */
    Q_REQUIRED_RESULT QString cachePath() const;

    Q_REQUIRED_RESULT quint64 leaveRoom(const QString &roomID);
    Q_REQUIRED_RESULT quint64 hideRoom(const QString &roomID);
    Q_REQUIRED_RESULT quint64 clearUnreadMessages(const QString &roomID);

    Q_REQUIRED_RESULT quint64 informTypingStatus(const QString &room, bool typing, const QString &userName);

    void setServerUrl(const QString &url);
    void start();
    void setLoginJobId(quint64 jobid);

    Q_REQUIRED_RESULT LoginStatus loginStatus() const;

    Q_REQUIRED_RESULT quint64 toggleFavorite(const QString &roomID, bool favorite);
    Q_REQUIRED_RESULT quint64 createChannel(const QString &name, const QStringList &userList, bool readOnly);
    Q_REQUIRED_RESULT quint64 createPrivateGroup(const QString &name, const QStringList &userList);
    Q_REQUIRED_RESULT quint64 joinRoom(const QString &roomId, const QString &joinCode);
    Q_REQUIRED_RESULT quint64 openDirectChannel(const QString &userId);
    void subscribeRoomMessage(const QString &roomId);
    Q_REQUIRED_RESULT quint64 setDefaultStatus(User::PresenceStatus status);
    Q_REQUIRED_RESULT quint64 listEmojiCustom();
    Q_REQUIRED_RESULT quint64 createJitsiConfCall(const QString &roomId);
    Q_REQUIRED_RESULT quint64 userAutocomplete(const QString &pattern, const QString &exception);
    Q_REQUIRED_RESULT quint64 deleteMessage(const QString &messageId);
    Q_REQUIRED_RESULT quint64 eraseRoom(const QString &roomID);
    Q_REQUIRED_RESULT quint64 setRoomName(const QString &roomId, const QString &name);
    Q_REQUIRED_RESULT quint64 setRoomTopic(const QString &roomId, const QString &topic);
    Q_REQUIRED_RESULT quint64  setRoomDescription(const QString &roomId, const QString &description);
    Q_REQUIRED_RESULT quint64  setRoomAnnouncement(const QString &roomId, const QString &announcement);
    Q_REQUIRED_RESULT quint64  starMessage(const QString &messageId, const QString &rid, bool starred);
    Q_REQUIRED_RESULT quint64  setRoomIsReadOnly(const QString &roomId, bool readOnly);
    Q_REQUIRED_RESULT quint64  archiveRoom(const QString &roomId);
    Q_REQUIRED_RESULT quint64  setReaction(const QString &emoji, const QString &messageId);
    Q_REQUIRED_RESULT quint64  getUsersOfRoom(const QString &roomId, bool showAll);
    Q_REQUIRED_RESULT quint64  loadHistory(const QJsonArray &params);
    Q_REQUIRED_RESULT quint64  channelAndPrivateAutocomplete(const QString &pattern, const QString &exception);
    Q_REQUIRED_RESULT quint64  roomFiles(const QString &roomId);
    Q_REQUIRED_RESULT quint64  addUserToRoom(const QString &userId, const QString &roomId);
    Q_REQUIRED_RESULT quint64  inputChannelAutocomplete(const QString &pattern, const QString &exceptions);
    Q_REQUIRED_RESULT quint64  inputUserAutocomplete(const QString &pattern, const QString &exceptions);
    Q_REQUIRED_RESULT quint64  login(const QString &username, const QString &password);
    Q_REQUIRED_RESULT quint64  loginProvider(const QString &credentialToken, const QString &credentialSecret);
    Q_REQUIRED_RESULT quint64  messageSearch(const QString &rid, const QString &pattern);
    Q_REQUIRED_RESULT quint64  unBlockUser(const QString &rid, const QString &userId);
    Q_REQUIRED_RESULT quint64  blockUser(const QString &rid, const QString &userId);
    Q_REQUIRED_RESULT quint64  disableNotifications(const QString &roomId, bool disabled);
    Q_REQUIRED_RESULT quint64  hideUnreadStatus(const QString &roomId, bool disabled);
    Q_REQUIRED_RESULT quint64  audioNotifications(const QString &roomId, const QString &value);
    Q_REQUIRED_RESULT quint64  mobilePushNotifications(const QString &roomId, const QString &value);
    Q_REQUIRED_RESULT quint64  desktopNotifications(const QString &roomId, const QString &value);
    Q_REQUIRED_RESULT quint64  emailNotifications(const QString &roomId, const QString &value);
    Q_REQUIRED_RESULT quint64  unreadAlert(const QString &roomId, const QString &value);
    Q_REQUIRED_RESULT quint64  deleteFileMessage(const QString &roomId, const QString &fileid, const QString &channelType);
    Q_REQUIRED_RESULT quint64  setRoomType(const QString &roomId, bool privateChannel);
Q_SIGNALS:
    void connectedChanged();
    void loginStatusChanged();
    void disconnected();
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void removed(const QJsonObject &item);

    /**
     * @brief Emitted whenever a result is received
     *
     * @param id The ID received in the method() call
     * @param result The response sent by server
     */
    void result(quint64 id, const QJsonDocument &result);

private slots:
    void onWSConnected();
    void onTextMessageReceived(const QString &message);
    void onWSclosed();

private:
    Q_DISABLE_COPY(DDPClient)
    void initializeWebSocket();

    QUrl adaptUrl(const QString &url);

    void setLoginStatus(LoginStatus l);

    void pong();
    void executeSubsCallBack(const QJsonObject &root);

    QString mUrl;
    AbstractWebSocket *mWebSocket = nullptr;

    /**
     * @brief Unique message ID for each message sent over network
     */
    quint64 m_uid = 0;

    /**
     * @brief Stores callback function associated with each message
     *
     * @def QHash unsigned messageID and std::function<void (QJsonDocument)> pointer to callback
     */
    QHash <quint64, std::function<void(QJsonObject, RocketChatAccount *)> > m_callbackHash;

    quint64 m_loginJob = 0;
    LoginStatus m_loginStatus;

    bool m_connected = false;

    bool m_attemptedPasswordLogin = false;
    bool m_attemptedTokenLogin = false;

    /**
    * @brief Abstract queue for all requests regarding network management
    *
    * @def QPair QString method and QJsonDocument params
    */
    QQueue<QPair<QString, QJsonDocument> > m_messageQueue;

    friend class Ruqola;
    RocketChatMessage *mRocketChatMessage = nullptr;
    RocketChatAccount *mRocketChatAccount = nullptr;
};

#endif // DDPCLIENT_H
