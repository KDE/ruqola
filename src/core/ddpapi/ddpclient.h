/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include "rocketchatmessage.h"
#include "room.h"
#include <QAbstractSocket>
#include <QQueue>
#include <QSslError>
#include <functional>

class QJsonObject;
class QJsonDocument;
class RocketChatMessage;
class AbstractWebSocket;
class RocketChatAccount;
class DDPAuthenticationManager;
class DDPManager;
class LIBRUQOLACORE_EXPORT DDPClient : public QObject
{
    Q_OBJECT
public:
    enum MessageType {
        Persistent,
        Ephemeral,
    };

    explicit DDPClient(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~DDPClient() override;

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
    quint64 method(const QString &method,
                   const QJsonDocument &params,
                   const std::function<void(QJsonObject, RocketChatAccount *)> &callback,
                   DDPClient::MessageType messageType = DDPClient::Ephemeral);

    quint64 method(const RocketChatMessage::RocketChatMessageResult &result,
                   const std::function<void(QJsonObject, RocketChatAccount *)> &callback,
                   DDPClient::MessageType messageType = DDPClient::Ephemeral);

    /**
     * @brief Subscribes to a collection with name @param collection and parameters @param params
     *
     * @param collection The name of the collection
     * @param params The parameters
     */
    quint64 subscribe(const QString &collection, const QJsonArray &params);

    /**
     * @brief Registers a @class DDPManager object to an event, represented by
     *   a (collection name, event name) pair.
     *
     * @param collection the name of the collection to which the event belongs
     * @param event the name of the event to which subsribe the @class DDPManager object
     * @param ddpManager the @class DDPManager instance that will process event messages
     * @param subscriptionId a manager specific id that helps the manager keep track of
     *   the subscription
     */
    void registerSubscriber(const QString &collection, const QString &event, DDPManager *ddpManager, int subscriptionId);

    /**
     * @brief Deregisters a @class DDPManager object to from a given event, represented by
     *   a (collection name, event name) pair.
     *
     *   The (collection, event) pair should be enough to deregister the subscribed manager,
     *   but the extra parameters are helpful to debug if subscription/unsubscription parameters
     *   don't match.
     *
     * @param collection the name of the collection to which the event belongs
     * @param event the name of the event to which subsribe the @class DDPManager object
     * @param ddpManager the @class DDPManager instance that will process event messages
     * @param subscriptionId a manager specific id that helps the manager keep track of
     *   the subscription
     */
    void deregisterSubscriber(const QString &collection, const QString &event, DDPManager *ddpManager, int subscriptionId);

    /**
     * @brief Calls a method on the websocket and registers the DDPManager invoking it
     *   for later response dispatching.
     *
     * @param method the method to be invoked
     * @param params parameters of the call
     * @param ddpmanager the manager invoking the call
     * @param operationId a manaager specific id to keep track of the operation invoked
     *   through the method
     */
    quint64 invokeMethodAndRegister(const QString &method, const QJsonArray &params, DDPManager *ddpManager, int operationId);

    /**
     * @brief Deregister an API manager from responses to a method represented by its id.
     *
     *   Silmilarly to deregisterSubscriber(), the @param methodId parameter is enough
     *   to deregister manager from method responses, but the other parameters are added
     *   for debugging purposes.
     *
     * @param methodId the id of the method generated at invocation
     * @param manager the current manager subscribed to method responses
     * @param operationId the manager specific operation bound to the method invocation
     */
    void deregisterFromMethodResponse(quint64 methodId, DDPManager *ddpManager, int operationId);

    /**
     * @brief Calls method to log in the user with valid username and password
     */
    void login();

    /**
     * @brief Tries logging in if the client is connected or just sets a flag
     *   so a login will be tried on connection.
     */
    void enqueueLogin();

    /**
     * @brief Check whether websocket is connected at url
     *
     * @return true if connected, else false
     */
    Q_REQUIRED_RESULT bool isConnected() const;

    /**
     * @brief Reconnects the websocket to new url
     */
    void onServerURLChange();

    /**
     * @brief Returns the queue used to cache unsent messages
     *
     *@return QQueue<QPair<QString,QJsonDocument>>, The m_messageQueue object
     */
    Q_REQUIRED_RESULT QQueue<QPair<QString, QJsonDocument>> messageQueue() const;

    /**
     * @brief Returns standard cache path
     *
     *@def QString path
     */
    Q_REQUIRED_RESULT QString cachePath() const;

    quint64 informTypingStatus(const QString &room, bool typing, const QString &userName);

    void setServerUrl(const QString &url);
    void start();

    Q_REQUIRED_RESULT DDPAuthenticationManager *authenticationManager() const;

    quint64 joinRoom(const QString &roomId, const QString &joinCode);
    quint64 openDirectChannel(const QString &userId);
    void subscribeRoomMessage(const QString &roomId);
    quint64 setDefaultStatus(User::PresenceStatus status);
    quint64 createJitsiConfCall(const QString &roomId);
    quint64 userAutocomplete(const QString &pattern, const QString &exception);
    quint64 loadHistory(const QJsonArray &params);
    quint64 inputChannelAutocomplete(const QString &pattern, const QString &exceptions, bool threadDialog);
    quint64 inputUserAutocomplete(const QString &pattern, const QString &exceptions, bool threadDialog);
    quint64 unBlockUser(const QString &rid, const QString &userId);
    quint64 blockUser(const QString &rid, const QString &userId);
    quint64 deleteFileMessage(const QString &roomId, const QString &fileid, Room::RoomType channelType);
    quint64 setRoomEncrypted(const QString &roomId, bool encrypted);
    quint64 roomNameExists(const QString &roomId);
    quint64 streamNotifyUserOtrEnd(const QString &roomId, const QString &userId);
    quint64 streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKey);
    quint64 streamNotifyUserOtrAcknowledge(const QString &roomId, const QString &userId, const QString &publicKey);
    quint64 deleteCustomSound(const QString &identifier);
    quint64 uploadCustomSound(const QByteArray &sound);
    void unsubscribe(quint64 registerId);
    quint64 listCustomSounds();
    quint64 enable2fa();
    quint64 disable2fa(const QString &code);
    quint64 validateTempToken2fa(const QString &code);
    quint64 regenerateCodes2fa(const QString &code);
    quint64 deleteOAuthApp(const QString &appId);
    quint64 addOAuthApp(const QString &name, bool active, const QString &redirectUrl);
    quint64 updateOAuthApp(const QString &name, bool active, const QString &redirectUrl);
    quint64 setAdminStatus(const QString &userId, bool admin);
    quint64 openRoom(const QString &roomId);
    quint64 getRoomById(const QString &roomId);
    quint64 bannerDismiss(const QString &bannerId);
    quint64 licenseGetModules();
Q_SIGNALS:
    void connecting();
    void connectedChanged();
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void removed(const QJsonObject &item);
    void socketError(QAbstractSocket::SocketError error, const QString &errorString);
    void disconnectedByServer();

    /**
     * @brief Emitted whenever a result is received
     *
     * @param id The ID received in the method() call
     * @param result The response sent by server
     */
    void result(quint64 id, const QJsonDocument &result);

private Q_SLOTS:
    void onWSConnected();
    void onTextMessageReceived(const QString &message);
    void onWSclosed();
    void onSslErrors(const QList<QSslError> &errors);

private:
    Q_DISABLE_COPY(DDPClient)
    void initializeWebSocket();
    void connectWebSocket();

    QUrl adaptUrl(const QString &url);

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
    QHash<quint64, std::function<void(QJsonObject, RocketChatAccount *)>> m_callbackHash;

    /**
     * @brief stores subscription handlers for a given event on a given collection
     *
     * @def The key of the hash is a QPair<QString, QString>, where the first element
     *   is the collection (stream) name, and the second is the event name.
     *   Each value is a pair (DDPManager, int), where the int value is an internal
     *   reference for the manager to match internally method calls with responses.
     *   When a subscription response is received by the client, it will be dispatched
     *   to the corresponding manager iff both the collection and the event name match.
     */
    QHash<QPair<QString, QString>, QPair<DDPManager *, int>> mEventSubscriptionHash;

    /**
     * @brief stores subscription handlers for a given event on a given collection
     *
     * @def The key of the hash is the generated id of the called method.
     *   When a method response is received by the client, it will be dispatched
     *   to the corresponding manager iff the method call id matches.
     */
    QHash<int, QPair<DDPManager *, int>> mMethodResponseHash;

    bool m_connected = false;

    /**
     * @brief Abstract queue for all requests regarding network management
     *
     * @def QPair QString method and QJsonDocument params
     */
    QQueue<QPair<QString, QJsonDocument>> m_messageQueue;

    friend class Ruqola;
    RocketChatMessage *mRocketChatMessage = nullptr;
    RocketChatAccount *mRocketChatAccount = nullptr;
    DDPAuthenticationManager *mAuthenticationManager = nullptr;

    bool mLoginEnqueued = false;
};
