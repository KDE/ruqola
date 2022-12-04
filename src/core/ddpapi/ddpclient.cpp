/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "ddpclient.h"
#include "customsound/customsoundsmanager.h"
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

void license_get_modules(const QJsonObject &root, RocketChatAccount *account)
{
    // qDebug() << " root " << root;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("License GetModule:") + QJsonDocument(root).toJson());
    }
    const QJsonArray obj = root.value(QLatin1String("result")).toArray();
    account->parseLicenses(obj);
}

void banner_dismiss(const QJsonObject &root, RocketChatAccount *account)
{
    // qDebug() << " root " << root;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Banner Dismiss:") + QJsonDocument(root).toJson());
    }
}

void list_custom_sounds(const QJsonObject &root, RocketChatAccount *account)
{
    qDebug() << " root " << root;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("list custom sounds:") + QJsonDocument(root).toJson());
    }
    const QJsonArray obj = root.value(QLatin1String("result")).toArray();
    account->customSoundManager()->parseCustomSounds(obj);
}

void delete_oauth_app(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Delete Oauth app:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    qDebug() << "delete_oauth_app root " << root;
}

void update_oauth_app(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Update Oauth App:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    account->setOauthAppUpdated(obj);
    qDebug() << "update_oauth_app root " << root;
}

void add_oauth_app(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Add Oauth App:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    account->setOauthAppAdded(obj);
}

void admin_status(const QJsonObject &root, RocketChatAccount *account)
{
    qDebug() << " root " << root;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Admin Status:") + QJsonDocument(root).toJson());
    }
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

void validateTempToken_2fa(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Validate Temp Token 2FA:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    account->totpVerify(obj);
}

void disable_2fa(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Disable 2FA:") + QJsonDocument(root).toJson());
    }
    account->totpDisabledVerify(root);
}

void regenerateCodes_2fa(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Regenerate Codes 2FA:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    // TODO
    qDebug() << " regenerateCodes_2fa " << root;
}

void enable_2fa(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Enable 2FA:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    account->generate2FaTotp(obj);
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
    // qDebug() << " void open_direct_channel(const QJsonObject &root, RocketChatAccount *account)" << obj;
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

void get_room_by_id(const QJsonObject &obj, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Get Room By ID :") + QJsonDocument(obj).toJson());
    }
}

void open_room(const QJsonObject &obj, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Open Room :") + QJsonDocument(obj).toJson());
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

    const QJsonArray params2{QJsonValue(QStringLiteral("%1/%2").arg(roomId, QStringLiteral("deleteMessage")))};
    subscribe(QStringLiteral("stream-notify-room"), params2);
    const QJsonArray params3{QJsonValue(QStringLiteral("%1/%2").arg(roomId, QStringLiteral("deleteMessageBulk")))};
    subscribe(QStringLiteral("stream-notify-room"), params3);
    const QJsonArray params4{QJsonValue(QStringLiteral("%1/%2").arg(roomId, QStringLiteral("typing")))};
    subscribe(QStringLiteral("stream-notify-room"), params4);
#ifdef USE_NEW_SUBSCRIBER // Use it when we depend against RC 4.0
    const QJsonArray params5{QJsonValue(QStringLiteral("%1/%2").arg(roomId, QStringLiteral("user-activity")))}; // It seems that it's the new "typing"
    subscribe(QStringLiteral("stream-notify-room"), params5);
#endif
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

quint64 DDPClient::openRoom(const QString &roomId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->openRoom(roomId, m_uid);
    return method(result, open_room, DDPClient::Persistent);
}

quint64 DDPClient::getRoomById(const QString &roomId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->getRoomById(roomId, m_uid);
    return method(result, get_room_by_id, DDPClient::Persistent);
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

quint64 DDPClient::listCustomSounds()
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->listCustomSounds(m_uid);
    return method(result, list_custom_sounds, DDPClient::Persistent);
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

quint64 DDPClient::enable2fa()
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->enable2fa(m_uid);
    return method(result, enable_2fa, DDPClient::Persistent);
}

quint64 DDPClient::disable2fa(const QString &code)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->disable2fa(code, m_uid);
    return method(result, disable_2fa, DDPClient::Persistent);
}

quint64 DDPClient::regenerateCodes2fa(const QString &code)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->regenerateCodes2fa(code, m_uid);
    return method(result, regenerateCodes_2fa, DDPClient::Persistent);
}

quint64 DDPClient::validateTempToken2fa(const QString &code)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->validateTempToken2fa(code, m_uid);
    return method(result, validateTempToken_2fa, DDPClient::Persistent);
}

quint64 DDPClient::streamNotifyUserOtrAcknowledge(const QString &roomId, const QString &userId, const QString &publicKey)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->streamNotifyUserOtrAcknowledge(roomId, userId, publicKey, m_uid);
    qDebug() << "streamNotifyUserOtrAcknowledge result " << result;
    return method(result, otr_end, DDPClient::Persistent);
}

quint64 DDPClient::addOAuthApp(const QString &name, bool active, const QString &redirectUrl)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->addOAuthApp(name, active, redirectUrl, m_uid);
    return method(result, add_oauth_app, DDPClient::Persistent);
}

quint64 DDPClient::updateOAuthApp(const QString &name, bool active, const QString &redirectUrl)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->updateOAuthApp(name, active, redirectUrl, m_uid);
    return method(result, update_oauth_app, DDPClient::Persistent);
}

quint64 DDPClient::blockUser(const QString &rid, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->blockUser(rid, userId, m_uid);
    return method(result, block_user, DDPClient::Persistent);
}

quint64 DDPClient::setAdminStatus(const QString &userId, bool admin)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setAdminStatus(userId, admin, m_uid);
    return method(result, admin_status, DDPClient::Persistent);
}

quint64 DDPClient::deleteOAuthApp(const QString &appId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->deleteOAuthApp(appId, m_uid);
    return method(result, delete_oauth_app, DDPClient::Persistent);
}

quint64 DDPClient::bannerDismiss(const QString &bannerId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->bannerDismiss(bannerId, m_uid);
    return method(result, banner_dismiss, DDPClient::Persistent);
}

quint64 DDPClient::licenseGetModules()
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->licenseGetModules(m_uid);
    return method(result, license_get_modules, DDPClient::Persistent);
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
        newParams.append(std::move(obj));
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
    const QByteArray serialize = QJsonDocument(protocol).toJson(QJsonDocument::Compact);
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
    if (normalClose) {
        Q_EMIT disconnectedByServer();
    } else {
        qCWarning(RUQOLA_DDPAPI_LOG) << "WebSocket CLOSED reason:" << mWebSocket->closeReason() << " error: " << mWebSocket->error()
                                     << " close code : " << mWebSocket->closeCode() << " error string " << mWebSocket->errorString();
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
