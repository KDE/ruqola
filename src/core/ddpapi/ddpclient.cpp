/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ddpclient.h"
#include "config-ruqola.h"
#include "messagequeue.h"
#include "plugins/pluginauthenticationinterface.h"
#include "rocketchataccountsettings.h"
#include "ruqola.h"
#include "ruqola_ddpapi_command_debug.h"
#include "ruqola_ddpapi_debug.h"
#include "ruqola_reconnect_core_debug.h"
#include "ruqolawebsocket.h"
#include "utils.h"

#include "authenticationmanager/ddpauthenticationmanager.h"
#include "ddpapi/ddpmanager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

DDPClient::DDPClient(QObject *parent)
    : QObject(parent)
    , mUid(1)
    , mRocketChatMessage(new RocketChatMessage)
    , mAuthenticationManager(new DDPAuthenticationManager(this, this))
{
}

DDPClient::~DDPClient()
{
    for (const qint64 id : std::as_const(mSubscribeIdentifiers)) {
        unsubscribe(id);
    }
    if (mWebSocket) {
        disconnect(mWebSocket, &AbstractWebSocket::disconnected, this, &DDPClient::onWSclosed);
        mWebSocket->close();
        delete mWebSocket;
        mWebSocket = nullptr;
    }
    delete mRocketChatMessage;
    mRocketChatMessage = nullptr;
}

void DDPClient::setServerUrl(const QString &url)
{
    mUrl = url;
}

void DDPClient::initializeWebSocket()
{
    mWebSocket->ignoreSslErrors();
    connect(mWebSocket, &AbstractWebSocket::connected, this, &DDPClient::onWSConnected);
    connect(mWebSocket, &AbstractWebSocket::textMessageReceived, this, &DDPClient::onTextMessageReceived);
    connect(mWebSocket, &AbstractWebSocket::disconnected, this, &DDPClient::onWSclosed);
    connect(mWebSocket, &AbstractWebSocket::socketError, this, &DDPClient::socketError);
    connect(mWebSocket, &AbstractWebSocket::sslErrors, this, &DDPClient::onSslErrors);
}

void DDPClient::start()
{
    if (!mWebSocket) {
        mWebSocket = new RuqolaWebSocket(mDDPClientAccountParameter->logger, this);
        initializeWebSocket();
    }
    if (!mUrl.isEmpty()) {
        const QUrl serverUrl = adaptUrl(mUrl);
        if (serverUrl.isValid()) {
            mWebSocket->openUrl(serverUrl);
            qCDebug(RUQOLA_RECONNECT_LOG) << "Trying to connect to URL" << serverUrl;
            Q_EMIT connecting();
        }
    } else {
        qCDebug(RUQOLA_DDPAPI_LOG) << "url is empty";
    }
}

qint64 DDPClient::loadPublicSettings(qint64 timeStamp)
{
    // https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-public-settings
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
        // qDebug() << "RocketChatBackend::loadPublicSettings load from: " << params;
        return method(u"public-settings/get"_s, params, DDPClient::MethodRequestedType::UpdatePublicsettings);
    } else {
        return method(u"public-settings/get"_s, params, DDPClient::MethodRequestedType::PublicSettings);
    }
}

void DDPClient::connectWebSocket()
{
    mWebSocket->openUrl(adaptUrl(mUrl));
    qCDebug(RUQOLA_RECONNECT_LOG) << "Reconnecting" << mUrl;
}

QUrl DDPClient::adaptUrl(const QString &url)
{
    return Utils::generateServerUrl(url);
}

void DDPClient::onServerURLChange()
{
    if (mDDPClientAccountParameter->settings->serverUrl() != mUrl || !mWebSocket->isValid()) {
        if (mWebSocket->isValid()) {
            mWebSocket->flush();
            mWebSocket->close();
        }
        mUrl = mDDPClientAccountParameter->settings->serverUrl();
        connectWebSocket();
    }
}

DDPAuthenticationManager *DDPClient::authenticationManager() const
{
    return mAuthenticationManager;
}

bool DDPClient::isConnected() const
{
    return mConnected;
}

QQueue<QPair<QString, QJsonDocument>> DDPClient::messageQueue() const
{
    return mMessageQueue;
}

void DDPClient::subscribeRoomMessage(const QByteArray &roomId)
{
    const QString rId = QString::fromLatin1(roomId);
    QJsonArray params;
    params.append(QJsonValue(rId));
    mSubscribeIdentifiers.append(subscribe(u"stream-room-messages"_s, params));

    const QStringList listStreamNotifierRoom{
        u"deleteMessage"_s,
        u"deleteMessageBulk"_s,
        u"user-activity"_s,
    };
    for (const QString &str : listStreamNotifierRoom) {
        const QJsonArray params2{QJsonValue(u"%1/%2"_s.arg(rId, str))};
        mSubscribeIdentifiers.append(subscribe(u"stream-notify-room"_s, params2));
    }
}

quint64 DDPClient::openDirectChannel(const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->openDirectChannel(userId, mUid);
    return method(result, DDPClient::MethodRequestedType::OpenDirectRoom, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::openRoom(const QByteArray &roomId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->openRoom(roomId, mUid);
    return method(result, DDPClient::MethodRequestedType::OpenRoom, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::setDefaultStatus(User::PresenceStatus status)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setDefaultStatus(status, mUid);
    return method(result, DDPClient::MethodRequestedType::ChangeDefaultStatus, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::getRooms(const QJsonObject &params)
{
    return method(u"rooms/get"_s, params, MethodRequestedType::RoomsParsing, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::createJitsiConfCall(const QByteArray &roomId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->createJitsiConfCall(roomId, mUid);
    return method(result, DDPClient::MethodRequestedType::CreateJitsiConfCall, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::inputChannelAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, bool threadDialog)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->inputChannelAutocomplete(roomId, pattern, exceptions, mUid);
    if (threadDialog) {
        return method(result, DDPClient::MethodRequestedType::InputUserChannelAutocompleteThread, DDPClient::MessageType::Persistent);
    } else {
        return method(result, DDPClient::MethodRequestedType::InputUserChannelAutocomplete, DDPClient::MessageType::Persistent);
    }
}

quint64 DDPClient::inputUserAutocomplete(const QByteArray &roomId, const QString &pattern, const QString &exceptions, bool threadDialog)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->inputUserAutocomplete(roomId, pattern, exceptions, mUid);
    if (threadDialog) {
        return method(result, DDPClient::MethodRequestedType::InputUserChannelAutocompleteThread, DDPClient::MessageType::Persistent);
    } else {
        return method(result, DDPClient::MethodRequestedType::InputUserChannelAutocomplete, DDPClient::MessageType::Persistent);
    }
}

quint64 DDPClient::unBlockUser(const QString &rid, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->unblockUser(rid, userId, mUid);
    return method(result, DDPClient::MethodRequestedType::UnBlockUser, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::listCustomSounds()
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->listCustomSounds(mUid);
    return method(result, DDPClient::MethodRequestedType::ListCustomSounds, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::deleteCustomSound(const QByteArray &identifier)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->deleteCustomSound(identifier, mUid);
    return method(result, DDPClient::MethodRequestedType::DeleteCustomSound, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::uploadCustomSound(const QByteArray &sound)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->uploadCustomSound(sound, mUid);
    return method(result, DDPClient::MethodRequestedType::UpdateCustomSound, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::streamNotifyUserOtrEnd(const QString &roomId, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->streamNotifyUserOtrEnd(roomId, userId, mUid);
    // qDebug() << " result " << result;
    return method(result, DDPClient::MethodRequestedType::OtrEnd, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKey)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->streamNotifyUserOtrHandshake(userFrom, userTo, publicKey, mUid);
    return method(result, DDPClient::MethodRequestedType::OtrEnd, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::enable2fa()
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->enable2fa(mUid);
    return method(result, DDPClient::MethodRequestedType::Enable2fa, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::disable2fa(const QString &code)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->disable2fa(code, mUid);
    return method(result, DDPClient::MethodRequestedType::Disable2fa, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::regenerateCodes2fa(const QString &code)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->regenerateCodes2fa(code, mUid);
    return method(result, DDPClient::MethodRequestedType::RegenerateCodes2fa, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::validateTempToken2fa(const QString &code)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->validateTempToken2fa(code, mUid);
    return method(result, DDPClient::MethodRequestedType::ValidateTempToken2fa, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::streamNotifyUserOtrAcknowledge(const QByteArray &roomId, const QByteArray &userId, const QString &publicKey)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->streamNotifyUserOtrAcknowledge(roomId, userId, publicKey, mUid);
    // qDebug() << "streamNotifyUserOtrAcknowledge result " << result;
    return method(result, DDPClient::MethodRequestedType::OtrEnd, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::blockUser(const QString &rid, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->blockUser(rid, userId, mUid);
    return method(result, DDPClient::MethodRequestedType::BlockUser, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::setAdminStatus(const QByteArray &userId, bool admin)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setAdminStatus(userId, admin, mUid);
    return method(result, DDPClient::MethodRequestedType::AdminStatus, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::bannerDismiss(const QByteArray &bannerId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->bannerDismiss(bannerId, mUid);
    return method(result, DDPClient::MethodRequestedType::BannerDismiss, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::videoConferenceAccepted(const QString &roomId, const QString &callId, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->videoConferenceAccepted(roomId, callId, userId, mUid);
    return method(result, DDPClient::MethodRequestedType::VideoConferenceAccepted, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::videoConferenceRejected(const QString &roomId, const QString &callId, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->videoConferenceRejected(roomId, callId, userId, mUid);
    return method(result, DDPClient::MethodRequestedType::VideoConferenceRejected, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::videoConferenceConfirmed(const QString &roomId, const QString &callId, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->videoConferenceConfirmed(roomId, callId, userId, mUid);
    return method(result, DDPClient::MethodRequestedType::VideoConferenceConfirmed, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::videoConferenceCall(const QString &roomId, const QString &callId, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->videoConferenceCall(roomId, callId, userId, mUid);
    return method(result, DDPClient::MethodRequestedType::VideoConferenceCall, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::informTypingStatus(const QByteArray &roomId, bool typing, const QString &userName)
{
    if (!mWebSocket) { // seems to happen when the server is restarted
        return -1;
    }
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->informTypingStatus(roomId, userName, typing, mUid);
    const qint64 bytes = mWebSocket->sendTextMessage(result.result);
    if (bytes < result.result.length()) {
        qCDebug(RUQOLA_DDPAPI_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << result.result;
    }
    const quint64 value = mUid;
    mUid++;
    return value;
}

quint64 DDPClient::method(const RocketChatMessage::RocketChatMessageResult &result, MethodRequestedType methodRequestedType, DDPClient::MessageType messageType)
{
    const qint64 bytes = mWebSocket->sendTextMessage(result.result);
    if (bytes < result.result.length()) {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();

        if (messageType == DDPClient::MessageType::Persistent) {
            mMessageQueue.enqueue(qMakePair(result.method, result.jsonDocument));
            mDDPClientAccountParameter->messageQueue->processQueue();
        }
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << result.result;
    }

    mMethodRequestedTypeHash[mUid] = methodRequestedType;

    const quint64 value = mUid;
    mUid++;
    return value;
}

void DDPClient::setDDPClientAccountParameter(DDPClientAccountParameter *newDDPClientAccountParameter)
{
    mDDPClientAccountParameter.reset(newDDPClientAccountParameter);
}

quint64 DDPClient::method(const QString &m, const QJsonArray &params, DDPClient::MessageType messageType)
{
    return method(m, params, MethodRequestedType::ShowDebug, messageType);
}

QJsonObject DDPClient::generateJsonObject(const QString &method, const QJsonArray &params)
{
    return RocketChatMessage::generateJsonObject(method, params, mUid++);
}

quint64
DDPClient::storeInQueue(const RocketChatMessage::RocketChatMessageResult &result, MethodRequestedType methodRequestedType, DDPClient::MessageType messageType)
{
    const qint64 bytes = mWebSocket->sendTextMessage(result.result);
    if (bytes < result.result.length()) {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();

        if (messageType == DDPClient::MessageType::Persistent) {
            mMessageQueue.enqueue(qMakePair(result.method, result.jsonDocument));
            mDDPClientAccountParameter->messageQueue->processQueue();
        }
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << result.result;
    }

    mMethodRequestedTypeHash[mUid] = methodRequestedType;

    const quint64 uidCurrent = mUid;
    mUid++;
    return uidCurrent;
}

quint64 DDPClient::method(const QString &methodName, const QJsonObject &params, MethodRequestedType methodRequestedType, DDPClient::MessageType messageType)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->generateMethod(methodName, params, mUid);
    return storeInQueue(result, methodRequestedType, messageType);
}

quint64 DDPClient::method(const QString &methodName, const QJsonArray &params, MethodRequestedType methodRequestedType, DDPClient::MessageType messageType)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->generateMethod(methodName, params, mUid);
    return storeInQueue(result, methodRequestedType, messageType);
}

void DDPClient::unsubscribe(quint64 registerId)
{
    const RocketChatMessage::RocketChatMessageResult resultUnsubscribe = mRocketChatMessage->unsubscribe(registerId);
    method(resultUnsubscribe, MethodRequestedType::Unsubscribe, DDPClient::MessageType::Persistent);
}

quint64 DDPClient::subscribe(const QString &collection, const QJsonArray &params)
{
    if (!mWebSocket) { // seems to happen when the server is restarted
        return -1;
    }
    const quint64 registerId = mUid;
    QJsonObject json;
    json["msg"_L1] = u"sub"_s;
    json["id"_L1] = QString::number(mUid);
    json["name"_L1] = collection;

    QJsonArray newParams = params;

    const QJsonArray args;
    QJsonObject obj;
    obj["useCollection"_L1] = false;
    obj["args"_L1] = args;
    newParams.append(std::move(obj));

    json["params"_L1] = newParams;
    qCDebug(RUQOLA_DDPAPI_LOG) << "subscribe: json " << json << "m_uid " << mUid;
    const qint64 bytes = mWebSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact)));
    if (bytes < json.length()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCWarning(RUQOLA_DDPAPI_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << json;
    }
    mUid++;
    mSubscribeIdentifiers.append(registerId);
    return registerId;
}

quint64 DDPClient::invokeMethodAndRegister(const QString &methodName, const QJsonArray &params, DDPManager *ddpManager, int operationId)
{
    qCDebug(RUQOLA_DDPAPI_LOG) << "invoked with" << methodName << params;
    mMethodResponseHash[mUid] = QPair<DDPManager *, int>(ddpManager, operationId);
    return method(methodName, params);
}

void DDPClient::deregisterFromMethodResponse(quint64 methodId, DDPManager *ddpManager, int operationId)
{
    if (!mMethodResponseHash.contains(methodId)) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "No API manager is registered to this method's responses. Method id:" << methodId;
        return;
    }

    const auto registerParams = mMethodResponseHash.value(methodId);
    const QPair<DDPManager *, int> deregisterParams{ddpManager, operationId};
    if (registerParams != deregisterParams) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Registration parameters for this method don't match the ones in the unregister request.";
        qCWarning(RUQOLA_DDPAPI_LOG).space() << "Method ID: " << methodId << ", registration parameters: " << registerParams
                                             << ", deregistration parameters: " << deregisterParams;
        return;
    }

    qCDebug(RUQOLA_DDPAPI_LOG) << "Registration to method" << methodId << "was removed successfully.";
    mMethodResponseHash.remove(methodId);
}

void DDPClient::onTextMessageReceived(const QString &message)
{
    const QJsonDocument response = QJsonDocument::fromJson(message.toUtf8());
    if (!response.isNull() && response.isObject()) {
        QJsonObject root = response.object();

        const QString messageType = root.value("msg"_L1).toString();

        if (messageType == "updated"_L1) {
            // nothing to do.
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << " message updated ! not implemented yet" << response;
        } else if (messageType == "result"_L1) {
            const quint64 id = root.value("id"_L1).toString().toULongLong();

            // Checking first if any of the new DDPManager claimed the result,
            // otherwise defaulting to old behaviour.
            if (mMethodResponseHash.contains(id)) {
                const QPair<DDPManager *, int> managerOperationPair = mMethodResponseHash.value(id);
                managerOperationPair.first->processMethodResponse(managerOperationPair.second, root);

                deregisterFromMethodResponse(id, managerOperationPair.first, managerOperationPair.second);
                return;
            }

            if (mMethodRequestedTypeHash.contains(id)) {
                Q_EMIT methodRequested(root, mMethodRequestedTypeHash.take(id));
            }
        } else if (messageType == "connected"_L1) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << " Connected!";
            mConnected = true;
            Q_EMIT connectedChanged(true);
        } else if (messageType == "error"_L1) {
            qWarning() << mDDPClientAccountParameter->accountName << " ERROR!!" << message;
        } else if (messageType == "ping"_L1) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << "Ping - Pong";
            pong();
        } else if (messageType == "added"_L1) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << "ADDING element" << response;
            Q_EMIT added(root);
        } else if (messageType == "changed"_L1) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << "Changed element" << response;
            Q_EMIT changed(root);
        } else if (messageType == "ready"_L1) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << "READY element" << response;
            executeSubsCallBack(root);
        } else if (messageType == "removed"_L1) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << "REMOVED element" << response;
            Q_EMIT removed(root);
        } else if (messageType == "nosub"_L1) {
            const QString id = root.value("id"_L1).toString();
            qCDebug(RUQOLA_DDPAPI_LOG) << mDDPClientAccountParameter->accountName << "Unsubscribe element" << message << id;
            const QJsonObject errorObj = root["error"_L1].toObject();
            if (!errorObj.isEmpty()) {
                qWarning() << mDDPClientAccountParameter->accountName << "Error unsubscribing from" << id;
                qWarning() << mDDPClientAccountParameter->accountName << "ERROR: " << errorObj["error"_L1].toString();
                qWarning() << mDDPClientAccountParameter->accountName << "Message: " << errorObj["message"_L1].toString();
                qWarning() << mDDPClientAccountParameter->accountName << "Reason: " << errorObj["reason"_L1].toString();
                qWarning() << mDDPClientAccountParameter->accountName << "-- Error found END --";
            }
        } else {
            // The very first message we receive is {"server_id":"0"}, can't find it in the spec, just ignore it.
            if (messageType.isEmpty() && !root.value("server_id"_L1).isUndefined()) {
                return;
            }
            qWarning() << mDDPClientAccountParameter->accountName << "received something unhandled:" << messageType << message;
        }
    } else {
        qWarning() << mDDPClientAccountParameter->accountName << "received something unhandled unknown " << message;
    }
}

void DDPClient::login()
{
    if (!Ruqola::useRestApiLogin()) {
        if (auto interface = mDDPClientAccountParameter->defaultAuthenticationInterface) {
            interface->login();
        } else {
            qCWarning(RUQOLA_DDPAPI_LOG) << "No plugins loaded. Please verify your installation.";
        }
    }
}

void DDPClient::enqueueLogin()
{
    if (isConnected()) {
        login();
    } else {
        // if the connection is already in, it's enough to wait for the web socket to connect
        mLoginEnqueued = true;
        if (!mWebSocket->isValid()) {
            connectWebSocket();
        }
    }
}

void DDPClient::onWSConnected()
{
    qCDebug(RUQOLA_DDPAPI_LOG) << "Websocket connected at URL" << mUrl;

    QJsonArray supportedVersions;
    supportedVersions.append("1"_L1);
    QJsonObject protocol;
    protocol["msg"_L1] = u"connect"_s;
    protocol["version"_L1] = u"1"_s;
    protocol["support"_L1] = supportedVersions;
    const QByteArray serialize = QJsonDocument(protocol).toJson(QJsonDocument::Compact);
    const qint64 bytes = mWebSocket->sendTextMessage(QString::fromUtf8(serialize));
    if (bytes < serialize.length()) {
        qCWarning(RUQOLA_DDPAPI_COMMAND_LOG) << "onWSConnected: ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCWarning(RUQOLA_DDPAPI_COMMAND_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << serialize;
    }
    if (!Ruqola::useRestApiLogin()) {
        if (mLoginEnqueued) {
            login();
            mLoginEnqueued = false;
        }
    }
}

void DDPClient::onSslErrors(const QList<QSslError> &errors)
{
    qCDebug(RUQOLA_DDPAPI_LOG) << "SSL error" << errors.count();
    for (const QSslError &err : errors) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "error ssl type:" << err.errorString();
    }
    mWebSocket->ignoreSslErrors();
}

void DDPClient::onWSclosed()
{
    qDebug();
    const bool normalClose = mWebSocket->closeCode() == QWebSocketProtocol::CloseCodeNormal;
    if (normalClose) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "DDP: Normal close, set status to LoggedOutAndCleanedUp, emit disconnectedByServer";
        authenticationManager()->setLoginStatus(AuthenticationManager::LoggedOutAndCleanedUp);
        Q_EMIT disconnectedByServer();
    } else {
        qCWarning(RUQOLA_DDPAPI_LOG) << "WebSocket CLOSED reason:" << mWebSocket->closeReason() << " error: " << mWebSocket->error()
                                     << " close code : " << mWebSocket->closeCode() << " error string " << mWebSocket->errorString() << "Protocol version"
                                     << mWebSocket->version();
        authenticationManager()->setLoginStatus(AuthenticationManager::GenericError);
        Q_EMIT wsClosedSocketError();
    }

    mConnected = false;
    Q_EMIT connectedChanged(false);
}

void DDPClient::pong()
{
    QJsonObject pong;
    pong["msg"_L1] = u"pong"_s;
    mWebSocket->sendBinaryMessage(QJsonDocument(pong).toJson(QJsonDocument::Compact));
}

void DDPClient::executeSubsCallBack(const QJsonObject &root)
{
    const QJsonArray subs = root["subs"_L1].toArray();
    if (!subs.isEmpty()) {
        const quint64 id = subs.at(0).toString().toULongLong();
        if (mMethodRequestedTypeHash.contains(id)) {
            Q_EMIT methodRequested(root, mMethodRequestedTypeHash.take(id));
        }
    } else {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Problem with subs json " << root;
    }
}

void DDPClient::loadPermissionsAdministrator(qint64 timeStamp)
{
    // TODO implement + store in database
    if (!isConnected()) {
        return;
    }
    // https://developer.rocket.chat/docs/get-permissions
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
    }
    qDebug() << " params " << params;
    method(u"permissions/get"_s, params, MethodRequestedType::PermissionsAdministrator);
}

void DDPClient::loadPrivateSettingsAdministrator(qint64 timeStamp)
{
    if (!isConnected()) {
        return;
    }
    // https://developer.rocket.chat/docs/get-permissions
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
    }
    // qDebug() << " params " << params;
    method(u"private-settings/get"_s, params, MethodRequestedType::PrivatesettingsAdministrator);
}

void DDPClient::loadPublicSettingsAdministrator(qint64 timeStamp)
{
    if (!isConnected()) {
        return;
    }
    // https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-public-settings
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
    }
    // qDebug() << " params " << params;
    method(u"public-settings/get"_s, params, MethodRequestedType::PublicsettingsAdministrator);
}

void DDPClient::initializeSubscription(qint64 timeStamp)
{
    // https://developer.rocket.chat/apidocs/get-all-subscriptions?highlight=subscription%2Fget
    QJsonObject params;
    // TODO use timeStamp too
    params["$date"_L1] = QJsonValue(timeStamp); // get ALL rooms we've ever seen

    // Add Query parameters
    // updatedSince

    // Date and time from which to fetch changes. Format: ISO 8601 datetime. Optional seconds, optional milliseconds, optional timezone, always with colon time
    // separators. Example 2017-11-25T15:08:17.248Z

    method(u"subscriptions/get"_s, params, MethodRequestedType::GetsubscriptionParsing);
}

quint64 DDPClient::getThreadMessages(const QJsonObject &params)
{
    return method(u"getThreadMessages"_s, params, DDPClient::MethodRequestedType::GetThreadMessages);
}

#include "moc_ddpclient.cpp"
