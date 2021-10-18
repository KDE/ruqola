/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2021 Laurent Montel <montel@kde.org>
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

#include "ddpclient.h"
#include "connection.h"
#include "messagequeue.h"
#include "plugins/pluginauthenticationinterface.h"
#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include "ruqola_ddpapi_command_debug.h"
#include "ruqola_ddpapi_debug.h"
#include "ruqolalogger.h"
#include "ruqolawebsocket.h"
#include "utils.h"

#include "ddpapi/ddpauthenticationmanager.h"
#include "ddpapi/ddpmanager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace RuqolaTestWebSocket
{
LIBRUQOLACORE_EXPORT AbstractWebSocket *_k_ruqola_webSocket = nullptr;
}

void block_user(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Block User:") + QJsonDocument(root).toJson());
    }
}

void unblock_user(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("UnBlock User:") + QJsonDocument(root).toJson());
    }
}

void delete_custom_sound(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Delete Custom Sound:") + QJsonDocument(root).toJson());
    }
}

void update_custom_sound(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Update Custom Sound:") + QJsonDocument(root).toJson());
    }
}

void otr_end(const QJsonObject &root, RocketChatAccount *account)
{
    qDebug() << "otr_end  " << root;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Otr End:") + QJsonDocument(root).toJson());
    }
}

void input_user_channel_autocomplete(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Input channel/User autocomplete:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    account->inputTextManager()->inputTextCompleter(obj);
}

void room_name_exist(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Check if Room Name Exist:") + QJsonDocument(root).toJson());
    }
    Q_EMIT account->ddp()->result(root.value(QLatin1String("id")).toString().toULongLong(), QJsonDocument(root));
}

void input_user_channel_autocomplete_thread(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Input channel/User autocomplete thread dialog:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();

    account->inputThreadMessageTextManager()->inputTextCompleter(obj);
}

void process_backlog(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    // qCDebug(RUQOLA_DDPAPI_LOG) << obj.value(QLatin1String("messages")).toArray().size();
    account->rocketChatBackend()->processIncomingMessages(obj.value(QLatin1String("messages")).toArray(), true);
}

void change_room_settings(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Change Room Settings:") + QJsonDocument(root).toJson());
    }
}

void create_jitsi_conf_call(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Create Jitsi Conf Call:") + QJsonDocument(root).toJson());
    }
}

void open_direct_channel(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    qDebug() << " void open_direct_channel(const QJsonObject &root, RocketChatAccount *account)" << obj;
    if (!obj.isEmpty()) {
        const QString rid = obj.value(QLatin1String("rid")).toString();
        if (!rid.isEmpty()) {
            account->ddp()->subscribeRoomMessage(rid);
        }
    }
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Open Direct channel:") + QJsonDocument(root).toJson());
    }
}

void join_room(const QJsonObject &obj, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Join Room :") + QJsonDocument(obj).toJson());
    }
}

void change_default_status(const QJsonObject &obj, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Change Default Status :") + QJsonDocument(obj).toJson());
    }
}

void empty_callback(const QJsonObject &obj, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Empty call back :") + QJsonDocument(obj).toJson());
    } else {
        qCWarning(RUQOLA_DDPAPI_LOG) << "empty_callback " << obj;
    }
}

DDPClient::DDPClient(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , m_uid(1)
    , mRocketChatMessage(new RocketChatMessage)
    , mRocketChatAccount(account)
    , mAuthenticationManager(new DDPAuthenticationManager(this, this))
{
}

DDPClient::~DDPClient()
{
    disconnect(mWebSocket, &AbstractWebSocket::disconnected, this, &DDPClient::onWSclosed);
    mWebSocket->close();
    // Don't delete socket when we use specific socket.
    if (!RuqolaTestWebSocket::_k_ruqola_webSocket) {
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
        if (!RuqolaTestWebSocket::_k_ruqola_webSocket) {
            mWebSocket = new RuqolaWebSocket(mRocketChatAccount->ruqolaLogger(), this);
        } else {
            mWebSocket = RuqolaTestWebSocket::_k_ruqola_webSocket;
        }
        initializeWebSocket();
    }
    connect(mRocketChatAccount, &RocketChatAccount::serverUrlChanged, this, &DDPClient::onServerURLChange);

    if (!mUrl.isEmpty()) {
        const QUrl serverUrl = adaptUrl(mUrl);
        if (serverUrl.isValid()) {
            mWebSocket->openUrl(serverUrl);
            qCDebug(RUQOLA_DDPAPI_LOG) << "Trying to connect to URL" << serverUrl;
            Q_EMIT connecting();
        }
    } else {
        qCDebug(RUQOLA_DDPAPI_LOG) << "url is empty";
    }
}

void DDPClient::connectWebSocket()
{
    mWebSocket->openUrl(adaptUrl(mUrl));
    qCDebug(RUQOLA_DDPAPI_LOG) << "Reconnecting" << mUrl;
}

QUrl DDPClient::adaptUrl(const QString &url)
{
    return Utils::generateServerUrl(url);
}

void DDPClient::onServerURLChange()
{
    if (mRocketChatAccount->settings()->serverUrl() != mUrl || !mWebSocket->isValid()) {
        if (mWebSocket->isValid()) {
            mWebSocket->flush();
            mWebSocket->close();
        }
        mUrl = mRocketChatAccount->settings()->serverUrl();
        connectWebSocket();
    }
}

DDPAuthenticationManager *DDPClient::authenticationManager() const
{
    return mAuthenticationManager;
}

bool DDPClient::isConnected() const
{
    return m_connected;
}

QString DDPClient::cachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QQueue<QPair<QString, QJsonDocument>> DDPClient::messageQueue() const
{
    return m_messageQueue;
}

quint64 DDPClient::setRoomEncrypted(const QString &roomId, bool encrypted)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomEncrypted(roomId, encrypted, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

void DDPClient::subscribeRoomMessage(const QString &roomId)
{
    QJsonArray params;
    params.append(QJsonValue(roomId));
    subscribe(QStringLiteral("stream-room-messages"), params);

    const QJsonArray params2{QJsonValue(QStringLiteral("%1/%2").arg(roomId, QStringLiteral("deleteMessage"))), true};
    subscribe(QStringLiteral("stream-notify-room"), params2);
    const QJsonArray params3{QJsonValue(QStringLiteral("%1/%2").arg(roomId, QStringLiteral("deleteMessageBulk"))), true};
    subscribe(QStringLiteral("stream-notify-room"), params3);
    const QJsonArray params4{QJsonValue(QStringLiteral("%1/%2").arg(roomId, QStringLiteral("typing"))), true};
    subscribe(QStringLiteral("stream-notify-room"), params4);
}

quint64 DDPClient::openDirectChannel(const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->openDirectChannel(userId, m_uid);
    return method(result, open_direct_channel, DDPClient::Persistent);
}

quint64 DDPClient::deleteFileMessage(const QString &roomId, const QString &fileid, Room::RoomType channelType)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->deleteFileMessage(fileid, m_uid);

    std::function<void(QJsonObject, RocketChatAccount *)> callback = [roomId, channelType](const QJsonObject &root, RocketChatAccount *account) {
        if (account->ruqolaLogger()) {
            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Delete Attachment File:") + QJsonDocument(root).toJson());
        } else {
            qCDebug(RUQOLA_DDPAPI_LOG) << " parse users for room" << roomId;
        }
        account->roomFiles(roomId, channelType);
    };

    return method(result, callback, DDPClient::Persistent);
}

quint64 DDPClient::joinRoom(const QString &roomId, const QString &joinCode)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->joinRoom(roomId, joinCode, m_uid);
    return method(result, join_room, DDPClient::Persistent);
}

quint64 DDPClient::setDefaultStatus(User::PresenceStatus status)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setDefaultStatus(status, m_uid);
    return method(result, change_default_status, DDPClient::Persistent);
}

quint64 DDPClient::userAutocomplete(const QString &pattern, const QString &exception)
{
    const quint64 subscribeId = m_uid;
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->userAutocomplete(pattern, exception, subscribeId);
    std::function<void(QJsonObject, RocketChatAccount *)> callback = [=](const QJsonObject &root, RocketChatAccount *account) {
        if (account->ruqolaLogger()) {
            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("User AutoComplete:") + QJsonDocument(root).toJson());
        } else {
            qCDebug(RUQOLA_DDPAPI_LOG) << " User AutoComplete" << root;
        }
        account->insertCompleterUsers();

        const RocketChatMessage::RocketChatMessageResult resultUnsubscribe = mRocketChatMessage->unsubscribe(subscribeId);
        std::function<void(QJsonObject, RocketChatAccount *)> callbackUnsubscribeAutoComplete = [=](const QJsonObject &root, RocketChatAccount *account) {
            if (account->ruqolaLogger()) {
                account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Unsubscribe AutoComplete:") + QJsonDocument(root).toJson());
            } else {
                qDebug() << " Unsubscribe AutoComplete" << root;
                qCDebug(RUQOLA_DDPAPI_LOG) << " Unsubscribe AutoComplete" << root;
            }
        };
        method(resultUnsubscribe, callbackUnsubscribeAutoComplete, DDPClient::Persistent);
    };

    return method(result, callback, DDPClient::Persistent);
}

quint64 DDPClient::createJitsiConfCall(const QString &roomId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->createJitsiConfCall(roomId, m_uid);
    return method(result, create_jitsi_conf_call, DDPClient::Persistent);
}

quint64 DDPClient::roomNameExists(const QString &roomName)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->roomNameExists(roomName, m_uid);
    return method(result, room_name_exist, DDPClient::Persistent);
}

quint64 DDPClient::inputChannelAutocomplete(const QString &pattern, const QString &exceptions, bool threadDialog)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->inputChannelAutocomplete(pattern, exceptions, m_uid);
    if (threadDialog) {
        return method(result, input_user_channel_autocomplete_thread, DDPClient::Persistent);
    } else {
        return method(result, input_user_channel_autocomplete, DDPClient::Persistent);
    }
}

quint64 DDPClient::inputUserAutocomplete(const QString &pattern, const QString &exceptions, bool threadDialog)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->inputUserAutocomplete(pattern, exceptions, m_uid);
    if (threadDialog) {
        return method(result, input_user_channel_autocomplete_thread, DDPClient::Persistent);
    } else {
        return method(result, input_user_channel_autocomplete, DDPClient::Persistent);
    }
}

quint64 DDPClient::unBlockUser(const QString &rid, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->unblockUser(rid, userId, m_uid);
    return method(result, unblock_user, DDPClient::Persistent);
}

quint64 DDPClient::deleteCustomSound(const QString &identifier)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->deleteCustomSound(identifier, m_uid);
    return method(result, delete_custom_sound, DDPClient::Persistent);
}

quint64 DDPClient::uploadCustomSound(const QByteArray &sound)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->uploadCustomSound(sound, m_uid);
    return method(result, update_custom_sound, DDPClient::Persistent);
}

quint64 DDPClient::streamNotifyUserOtrEnd(const QString &roomId, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->streamNotifyUserOtrEnd(roomId, userId, m_uid);
    qDebug() << " result " << result;
    return method(result, otr_end, DDPClient::Persistent);
}

quint64 DDPClient::streamNotifyUserOtrHandshake(const QString &userFrom, const QString &userTo, const QString &publicKey)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->streamNotifyUserOtrHandshake(userFrom, userTo, publicKey, m_uid);
    return method(result, otr_end, DDPClient::Persistent);
}

quint64 DDPClient::streamNotifyUserOtrAcknowledge(const QString &roomId, const QString &userId, const QString &publicKey)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->streamNotifyUserOtrAcknowledge(roomId, userId, publicKey, m_uid);
    qDebug() << "streamNotifyUserOtrAcknowledge result " << result;
    return method(result, otr_end, DDPClient::Persistent);
}

quint64 DDPClient::blockUser(const QString &rid, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->blockUser(rid, userId, m_uid);
    return method(result, block_user, DDPClient::Persistent);
}

quint64 DDPClient::informTypingStatus(const QString &roomId, bool typing, const QString &userName)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->informTypingStatus(roomId, userName, typing, m_uid);
    const qint64 bytes = mWebSocket->sendTextMessage(result.result);
    if (bytes < result.result.length()) {
        qCDebug(RUQOLA_DDPAPI_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << result.result;
    }
    const quint64 value = m_uid;
    m_uid++;
    return value;
}

quint64 DDPClient::method(const RocketChatMessage::RocketChatMessageResult &result,
                          const std::function<void(QJsonObject, RocketChatAccount *)> &callback,
                          DDPClient::MessageType messageType)
{
    qint64 bytes = mWebSocket->sendTextMessage(result.result);
    if (bytes < result.result.length()) {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();

        if (messageType == DDPClient::Persistent) {
            m_messageQueue.enqueue(qMakePair(result.method, result.jsonDocument));
            mRocketChatAccount->messageQueue()->processQueue();
        }
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << result.result;
    }

    m_callbackHash[m_uid] = callback;

    const quint64 value = m_uid;
    m_uid++;
    return value;
}

quint64 DDPClient::method(const QString &m, const QJsonDocument &params, DDPClient::MessageType messageType)
{
    return method(m, params, empty_callback, messageType);
}

quint64 DDPClient::method(const QString &method,
                          const QJsonDocument &params,
                          const std::function<void(QJsonObject, RocketChatAccount *)> &callback,
                          DDPClient::MessageType messageType)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->generateMethod(method, params, m_uid);
    qint64 bytes = mWebSocket->sendTextMessage(result.result);
    if (bytes < result.result.length()) {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();

        if (messageType == DDPClient::Persistent) {
            m_messageQueue.enqueue(qMakePair(result.method, result.jsonDocument));
            mRocketChatAccount->messageQueue()->processQueue();
        }
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << result.result;
    }

    m_callbackHash[m_uid] = callback;

    const quint64 uidCurrent = m_uid;
    m_uid++;
    return uidCurrent;
}

void DDPClient::unsubscribe(quint64 registerId)
{
    const RocketChatMessage::RocketChatMessageResult resultUnsubscribe = mRocketChatMessage->unsubscribe(registerId);
    std::function<void(QJsonObject, RocketChatAccount *)> callbackUnsubscribeMethod = [=](const QJsonObject &root, RocketChatAccount *account) {
        if (account->ruqolaLogger()) {
            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Unsubscribe Method:") + QJsonDocument(root).toJson());
        } else {
            qDebug() << " Unsubscribe Method" << root;
            qCDebug(RUQOLA_DDPAPI_LOG) << " Unsubscribe Method" << root;
        }
    };
    method(resultUnsubscribe, callbackUnsubscribeMethod, DDPClient::Persistent);
}

quint64 DDPClient::subscribe(const QString &collection, const QJsonArray &params)
{
    quint64 registerId = m_uid;
    QJsonObject json;
    json[QStringLiteral("msg")] = QStringLiteral("sub");
    json[QStringLiteral("id")] = QString::number(m_uid);
    json[QStringLiteral("name")] = collection;

    QJsonArray newParams = params;

    if (mRocketChatAccount->needAdaptNewSubscriptionRC60()) {
        QJsonArray args;
        QJsonObject obj;
        obj[QStringLiteral("useCollection")] = false;
        obj[QStringLiteral("args")] = args;
        newParams.append(obj);
    }

    json[QStringLiteral("params")] = newParams;
    // qDebug() << "subscribe: json " << json;
    qint64 bytes = mWebSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact)));
    if (bytes < json.length()) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCWarning(RUQOLA_DDPAPI_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << json;
    }
    m_uid++;
    return registerId;
}

void DDPClient::registerSubscriber(const QString &collection, const QString &event, DDPManager *ddpManager, int subscriptionId)
{
    const QPair<QString, QString> &key{collection, event};

    if (mEventSubscriptionHash.contains(key)) {
        qCCritical(RUQOLA_DDPAPI_LOG) << "ERROR! Another manager is subscribed to this event, registration failed.";
        return;
    }

    mEventSubscriptionHash[key] = {ddpManager, subscriptionId};
    // Registering the client through its existing subscribe API
    // TODO: check how useCollection and args are used
    const QString params = QStringLiteral(R"([
    "%1",
    {
        "useCollection": false,
        "args": []
    }
])")
                               .arg(event);

    subscribe(collection, Utils::strToJsonArray(params));
}

void DDPClient::deregisterSubscriber(const QString &collection, const QString &event, DDPManager *ddpManager, int subscriptionId)
{
    const QPair<QString, QString> key{collection, event};

    if (!mEventSubscriptionHash.contains(key)) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "No DDPManager is subscribed to this event" << key;
        return;
    }

    const QPair<DDPManager *, int> subscriptionParams = mEventSubscriptionHash.value(key);
    const auto unsubscriptionParams = QPair<DDPManager *, int>{ddpManager, subscriptionId};
    if (subscriptionParams != unsubscriptionParams) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Unsubscription parameters don't match subscription parameters.";
        qCWarning(RUQOLA_DDPAPI_LOG).nospace() << "Subscription parameters: " << subscriptionParams << ", unsubscription parameters: " << unsubscriptionParams;
        return;
    }

    qCDebug(RUQOLA_DDPAPI_LOG) << "Subscription to event" << key << "was removed successfully.";
    mEventSubscriptionHash.remove(key);
}

quint64 DDPClient::invokeMethodAndRegister(const QString &methodName, const QJsonArray &params, DDPManager *ddpManager, int operationId)
{
    qCDebug(RUQOLA_DDPAPI_LOG) << Q_FUNC_INFO << "invoked with" << methodName << params;
    mMethodResponseHash[m_uid] = QPair<DDPManager *, int>(ddpManager, operationId);
    return method(methodName, QJsonDocument(params));
}

void DDPClient::deregisterFromMethodResponse(quint64 methodId, DDPManager *ddpManager, int operationId)
{
    if (!mMethodResponseHash.contains(methodId)) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "No API manager is registered to this method's responses. Method id:" << methodId;
        return;
    }

    const auto registerParams = mMethodResponseHash[methodId];
    const QPair<DDPManager *, int> deregisterParams{ddpManager, operationId};
    if (registerParams != deregisterParams) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Registration parameters for this method don't match the ones in the unregister request.";
        qCWarning(RUQOLA_DDPAPI_LOG).nospace() << "Method ID: " << methodId << ", registration parameters: " << registerParams
                                               << ", deregistration parameters: " << deregisterParams;
        return;
    }

    qCDebug(RUQOLA_DDPAPI_LOG) << "Registration to method" << methodId << "was removed successfully.";
    mMethodResponseHash.remove(methodId);
}

void DDPClient::onTextMessageReceived(const QString &message)
{
    QJsonDocument response = QJsonDocument::fromJson(message.toUtf8());
    if (!response.isNull() && response.isObject()) {
        QJsonObject root = response.object();

        const QString messageType = root.value(QLatin1String("msg")).toString();

        if (messageType == QLatin1String("updated")) {
            // nothing to do.
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << " message updated ! not implemented yet" << response;
        } else if (messageType == QLatin1String("result")) {
            quint64 id = root.value(QLatin1String("id")).toString().toULongLong();

            // Checking first if any of the new DDPManager claimed the result,
            // otherwise defaulting to old behaviour.
            if (mMethodResponseHash.contains(id)) {
                QPair<DDPManager *, int> managerOperationPair = mMethodResponseHash[id];
                managerOperationPair.first->processMethodResponse(managerOperationPair.second, root);
                return;
            }

            if (m_callbackHash.contains(id)) {
                std::function<void(QJsonObject, RocketChatAccount *)> callback = m_callbackHash.take(id);
                callback(root, mRocketChatAccount);
            }

            Q_EMIT result(id, QJsonDocument(root.value(QLatin1String("result")).toObject()));
        } else if (messageType == QLatin1String("connected")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << " Connected!";
            m_connected = true;
            Q_EMIT connectedChanged();
        } else if (messageType == QLatin1String("error")) {
            qWarning() << mRocketChatAccount->accountName() << " ERROR!!" << message;
        } else if (messageType == QLatin1String("ping")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << "Ping - Pong";
            pong();
        } else if (messageType == QLatin1String("added")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << "ADDING element" << response;
            Q_EMIT added(root);
        } else if (messageType == QLatin1String("changed")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << "Changed element" << response;
            Q_EMIT changed(root);
        } else if (messageType == QLatin1String("ready")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << "READY element" << response;
            executeSubsCallBack(root);
        } else if (messageType == QLatin1String("removed")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << "REMOVED element" << response;
            Q_EMIT removed(root);
        } else if (messageType == QLatin1String("nosub")) {
            const QString id = root.value(QStringLiteral("id")).toString();
            qCDebug(RUQOLA_DDPAPI_LOG) << mRocketChatAccount->accountName() << "Unsubscribe element" << message << id;
            const QJsonObject errorObj = root[QStringLiteral("error")].toObject();
            if (!errorObj.isEmpty()) {
                qWarning() << mRocketChatAccount->accountName() << "Error unsubscribing from" << id;
                qWarning() << mRocketChatAccount->accountName() << "ERROR: " << errorObj[QStringLiteral("error")].toString();
                qWarning() << mRocketChatAccount->accountName() << "Message: " << errorObj[QStringLiteral("message")].toString();
                qWarning() << mRocketChatAccount->accountName() << "Reason: " << errorObj[QStringLiteral("reason")].toString();
                qWarning() << mRocketChatAccount->accountName() << "-- Error found END --";
            }
        } else {
            // The very first message we receive is {"server_id":"0"}, can't find it in the spec, just ignore it.
            if (messageType.isEmpty() && !root.value(QStringLiteral("server_id")).isUndefined()) {
                return;
            }
            qWarning() << mRocketChatAccount->accountName() << "received something unhandled:" << messageType << message;
        }
    } else {
        qWarning() << mRocketChatAccount->accountName() << "received something unhandled unknown " << message;
    }
}

quint64 DDPClient::loadHistory(const QJsonArray &params)
{
    return method(QStringLiteral("loadHistory"), QJsonDocument(params), process_backlog);
}

void DDPClient::login()
{
    if (mRocketChatAccount->defaultAuthenticationInterface()) {
        mRocketChatAccount->defaultAuthenticationInterface()->login();
    } else {
        qCWarning(RUQOLA_DDPAPI_LOG) << "No plugins loaded. Please verify your installation.";
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
    supportedVersions.append(QLatin1String("1"));
    QJsonObject protocol;
    protocol[QStringLiteral("msg")] = QStringLiteral("connect");
    protocol[QStringLiteral("version")] = QStringLiteral("1");
    protocol[QStringLiteral("support")] = supportedVersions;
    QByteArray serialize = QJsonDocument(protocol).toJson(QJsonDocument::Compact);
    qint64 bytes = mWebSocket->sendTextMessage(QString::fromUtf8(serialize));
    if (bytes < serialize.length()) {
        qCWarning(RUQOLA_DDPAPI_COMMAND_LOG) << "onWSConnected: ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCWarning(RUQOLA_DDPAPI_COMMAND_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << serialize;
    }

    if (mLoginEnqueued) {
        login();
        mLoginEnqueued = false;
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
    const bool normalClose = mWebSocket->closeCode() == QWebSocketProtocol::CloseCodeNormal;
    if (!normalClose) {
        qCWarning(RUQOLA_DDPAPI_LOG) << "WebSocket CLOSED reason:" << mWebSocket->closeReason() << " error: " << mWebSocket->error()
                                     << " close code : " << mWebSocket->closeCode();
    }

    if (normalClose) {
        Q_EMIT disconnectedByServer();
    }

    m_connected = false;
    Q_EMIT connectedChanged();
}

void DDPClient::pong()
{
    QJsonObject pong;
    pong[QStringLiteral("msg")] = QStringLiteral("pong");
    mWebSocket->sendBinaryMessage(QJsonDocument(pong).toJson(QJsonDocument::Compact));
}

void DDPClient::executeSubsCallBack(const QJsonObject &root)
{
    const QJsonArray subs = root[QStringLiteral("subs")].toArray();
    if (!subs.isEmpty()) {
        const quint64 id = subs.at(0).toString().toULongLong();
        if (m_callbackHash.contains(id)) {
            std::function<void(QJsonObject, RocketChatAccount *)> callback = m_callbackHash.take(id);
            callback(root, mRocketChatAccount);
        }
    } else {
        qCWarning(RUQOLA_DDPAPI_LOG) << "Problem with subs json " << root;
    }
}

// Otr end
//["{\"msg\":\"method\",\"id\":\"22\",\"method\":\"stream-notify-user\",\"params\":[\"YbwG4T2uB3wZSZSKB/otr\",\"end\",{\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\",\"userId\":\"4faACeGzSvG7xMcTy\"}]}"]
// Otr handshake
//\"msg\":\"method\",\"id\":\"24\",\"method\":\"stream-notify-user\",\"params\":[\"YbwG4T2uB3wZSZSKB/otr\",\"handshake\",{\"roomId\":\"4faACeGzSvG7xMcTyYbwG4T2uB3wZSZSKB\",\"userId\":\"4faACeGzSvG7xMcTy\",\"publicKey\":\"{\\\
