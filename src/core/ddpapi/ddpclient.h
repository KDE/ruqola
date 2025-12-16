/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#pragma once

#include "libruqolacore_export.h"
#include "rocketchatmessage.h"
#include <QAbstractSocket>
#include <QQueue>
#include <QSslError>

class QJsonObject;
class QJsonDocument;
class RocketChatMessage;
class AbstractWebSocket;
class DDPAuthenticationManager;
class RuqolaLogger;
class RocketChatAccountSettings;
class DDPManager;
class MessageQueue;
class PluginAuthenticationInterface;
class LIBRUQOLACORE_EXPORT DDPClient : public QObject
{
    Q_OBJECT
public:
    enum class MessageType : uint8_t {
        Persistent,
        Ephemeral,
    };

    enum class MethodRequestedType : uint8_t {
        Unknown,
        PublicSettings,
        UpdatePublicsettings,
        ListCustomSounds,
        BlockUser,
        UnBlockUser,
        UpdateCustomSound,
        DeleteCustomSound,
        RegenerateCodes2fa,
        Enable2fa,
        Disable2fa,
        ValidateTempToken2fa,
        BannerDismiss,
        ChangeDefaultStatus,
        OpenRoom,
        OpenDirectRoom,
        CreateJitsiConfCall,
        InputUserChannelAutocompleteThread,
        InputUserChannelAutocomplete,
        OtrEnd,
        AdminStatus,
        VideoConferenceCall,
        VideoConferenceRejected,
        VideoConferenceAccepted,
        VideoConferenceConfirmed,
        PublicsettingsAdministrator,
        PrivatesettingsAdministrator,
        PermissionsAdministrator,
        RoomsParsing,
        GetsubscriptionParsing,
        Unsubscribe,
        ShowDebug,
        GetThreadMessages,
    };

    struct LIBRUQOLACORE_EXPORT DDPClientAccountParameter {
        DDPClientAccountParameter() = default;
        DDPClientAccountParameter(const QString &_accountName, RuqolaLogger *_logger, RocketChatAccountSettings *_settings, MessageQueue *_messageQueue)
            : accountName(_accountName)
            , logger(_logger)
            , settings(_settings)
            , messageQueue(_messageQueue)
        {
        }

        QString accountName;
        RuqolaLogger *logger = nullptr;
        RocketChatAccountSettings *settings = nullptr;
        MessageQueue *messageQueue = nullptr;
        PluginAuthenticationInterface *defaultAuthenticationInterface = nullptr;
    };

    explicit DDPClient(QObject *parent = nullptr);
    ~DDPClient() override;

    /**
     * @brief Call a method with name @param method and parameters @param params and @param messageType with an empty callback
     *
     * @param method The name of the method to call Rocket.Chat API for
     * @param params The parameters
     * @param messageType The type of message
     * @return unsigned int, the ID of the called method
     */
    quint64 method(const QString &method, const QJsonArray &params, DDPClient::MessageType messageType = DDPClient::MessageType::Ephemeral);

    /**
     * @brief Subscribes to a collection with name @param collection and parameters @param params
     *
     * @param collection The name of the collection
     * @param params The parameters
     */
    quint64 subscribe(const QString &collection, const QJsonArray &params);

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
    [[nodiscard]] bool isConnected() const;

    /**
     * @brief Reconnects the websocket to new url
     */
    void onServerURLChange();

    /**
     * @brief Returns the queue used to cache unsent messages
     *
     *@return QQueue<QPair<QString,QJsonDocument>>, The m_messageQueue object
     */
    [[nodiscard]] QQueue<QPair<QString, QJsonDocument>> messageQueue() const;

    quint64 informTypingStatus(const QByteArray &room, bool typing, const QString &userName);

    void setServerUrl(const QString &url);
    void start();

    [[nodiscard]] DDPAuthenticationManager *authenticationManager() const;

    quint64 openDirectChannel(const QString &userId);
    void subscribeRoomMessage(const QByteArray &roomId);
    quint64 setDefaultStatus(User::PresenceStatus status);
    quint64 createJitsiConfCall(const QByteArray &roomId);
    quint64 inputChannelAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, bool threadDialog);
    quint64 inputUserAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, bool threadDialog);
    quint64 unBlockUser(const QString &rid, const QString &userId);
    quint64 blockUser(const QString &rid, const QString &userId);
    quint64 streamNotifyUserOtrEnd(const QString &roomId, const QString &userId);
    quint64 streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKey);
    quint64 streamNotifyUserOtrAcknowledge(const QByteArray &roomId, const QByteArray &userId, const QString &publicKey);
    quint64 deleteCustomSound(const QByteArray &identifier);
    quint64 uploadCustomSound(const QByteArray &sound);
    void unsubscribe(quint64 registerId);
    quint64 listCustomSounds();
    quint64 enable2fa();
    quint64 disable2fa(const QString &code);
    quint64 validateTempToken2fa(const QString &code);
    quint64 regenerateCodes2fa(const QString &code);
    quint64 setAdminStatus(const QByteArray &userId, bool admin);
    quint64 openRoom(const QByteArray &roomId);
    quint64 bannerDismiss(const QByteArray &bannerId);
    quint64 videoConferenceAccepted(const QString &roomId, const QString &callId, const QString &userId);
    quint64 videoConferenceRejected(const QString &roomId, const QString &callId, const QString &userId);
    quint64 videoConferenceCall(const QString &roomId, const QString &callId, const QString &userId);
    quint64 videoConferenceConfirmed(const QString &roomId, const QString &callId, const QString &userId);
    [[nodiscard]] QJsonObject generateJsonObject(const QString &method, const QJsonArray &params);
    qint64 loadPublicSettings(qint64 timeStamp = -1);
    void loadPermissionsAdministrator(qint64 timeStamp = -1);
    void loadPrivateSettingsAdministrator(qint64 timeStamp = -1);
    void loadPublicSettingsAdministrator(qint64 timeStamp = -1);
    void initializeSubscription(qint64 timeStamp = -1);
    void setDDPClientAccountParameter(DDPClientAccountParameter *newDDPClientAccountParameter);

    quint64 getRooms(const QJsonObject &params);
    quint64 getThreadMessages(const QJsonObject &params);
Q_SIGNALS:
    void connecting();
    void connectedChanged(bool connected);
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void removed(const QJsonObject &item);
    void socketError(QAbstractSocket::SocketError error, const QString &errorString);
    void disconnectedByServer();
    void wsClosedSocketError();
    void methodRequested(const QJsonObject &obj, DDPClient::MethodRequestedType type);

private Q_SLOTS:
    void onWSConnected();
    void onTextMessageReceived(const QString &message);
    void onWSclosed();
    void onSslErrors(const QList<QSslError> &errors);

private:
    LIBRUQOLACORE_NO_EXPORT void initializeWebSocket();
    LIBRUQOLACORE_NO_EXPORT void connectWebSocket();

    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QUrl adaptUrl(const QString &url);

    LIBRUQOLACORE_NO_EXPORT void pong();
    LIBRUQOLACORE_NO_EXPORT void executeSubsCallBack(const QJsonObject &root);

    LIBRUQOLACORE_NO_EXPORT quint64 storeInQueue(const RocketChatMessage::RocketChatMessageResult &result,
                                                 MethodRequestedType methodRequestedType,
                                                 DDPClient::MessageType messageType);

    LIBRUQOLACORE_NO_EXPORT quint64 method(const QString &methodName,
                                           const QJsonObject &params,
                                           MethodRequestedType methodRequestedType,
                                           DDPClient::MessageType messageType = DDPClient::MessageType::Ephemeral);

    LIBRUQOLACORE_NO_EXPORT quint64 method(const QString &methodName,
                                           const QJsonArray &params,
                                           MethodRequestedType methodRequestedType,
                                           DDPClient::MessageType messageType);

    LIBRUQOLACORE_NO_EXPORT quint64 method(const RocketChatMessage::RocketChatMessageResult &result,
                                           MethodRequestedType methodRequestedType,
                                           DDPClient::MessageType messageType = DDPClient::MessageType::Ephemeral);

    QString mUrl;
    AbstractWebSocket *mWebSocket = nullptr;

    /**
     * @brief Unique message ID for each message sent over network
     */
    quint64 mUid = 0;

    QHash<quint64, MethodRequestedType> mMethodRequestedTypeHash;

    /**
     * @brief stores subscription handlers for a given event on a given collection
     *
     * @def The key of the hash is the generated id of the called method.
     *   When a method response is received by the client, it will be dispatched
     *   to the corresponding manager iff the method call id matches.
     */
    QHash<int, QPair<DDPManager *, int>> mMethodResponseHash;

    bool mConnected = false;

    /**
     * @brief Abstract queue for all requests regarding network management
     *
     * @def QPair QString method and QJsonDocument params
     */
    QQueue<QPair<QString, QJsonDocument>> mMessageQueue;

    friend class Ruqola;
    RocketChatMessage *mRocketChatMessage = nullptr;
    DDPAuthenticationManager *mAuthenticationManager = nullptr;
    std::unique_ptr<DDPClientAccountParameter> mDDPClientAccountParameter;
    QList<qint64> mSubscribeIdentifiers;
    bool mLoginEnqueued = false;
};
