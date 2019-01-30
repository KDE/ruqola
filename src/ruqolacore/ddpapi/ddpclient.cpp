/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>
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
#include "restapirequest.h"
#include "utils.h"
#include "ruqola_ddpapi_debug.h"
#include "ruqola_ddpapi_command_debug.h"
#include "rocketchatmessage.h"
#include "ruqolawebsocket.h"
#include "rocketchataccount.h"
#include "messagequeue.h"
#include "ruqolalogger.h"
#include "rocketchatbackend.h"
#include "plugins/pluginauthenticationinterface.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>

namespace RuqolaTestWebSocket {
LIBRUQOLACORE_EXPORT AbstractWebSocket *_k_ruqola_webSocket = nullptr;
}

void user_ignore(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("User Ignore:") + QJsonDocument(root).toJson());
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

void message_search(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Search Message:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    account->displaySearchedMessage(obj);
}

void input_user_channel_autocomplete(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Input channel/User autocomplete:") + QJsonDocument(root).toJson());
    }
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();

    account->inputTextCompleter(obj);
}

void process_backlog(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    //qCDebug(RUQOLA_DDPAPI_LOG) << obj.value(QLatin1String("messages")).toArray().size();
    account->rocketChatBackend()->processIncomingMessages(obj.value(QLatin1String("messages")).toArray());
}

void add_user_to_room(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Add User To Room:") + QJsonDocument(root).toJson());
    }
}

void star_message(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Star message:") + QJsonDocument(root).toJson());
    }
}

void change_notifications_settings(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Change Notifications Settings:") + QJsonDocument(root).toJson());
    }
}

void change_room_settings(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Change Room Settings:") + QJsonDocument(root).toJson());
    }
}

void erase_room(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Delete Room:") + QJsonDocument(root).toJson());
    }
    qDebug()<< "void erase_room(const QJsonObject &root, RocketChatAccount *account)"<<root;
}

void delete_message(const QJsonObject &root, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Delete Message:") + QJsonDocument(root).toJson());
    }
}

void channel_and_private_autocomplete(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    account->loadAutoCompleteChannel(obj);
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Channel And Private AutoComplete:") + QJsonDocument(root).toJson());
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

void list_emoji_custom(const QJsonObject &obj, RocketChatAccount *account)
{
    account->loadEmoji(obj);
    //qDebug() << " list emoji custom " << obj[QLatin1String("result")].toArray();
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Load Emoji Custom :") + QJsonDocument(obj).toJson());
    }
}

void empty_callback(const QJsonObject &obj, RocketChatAccount *account)
{
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Empty call back :") + QJsonDocument(obj).toJson());
    } else {
        qCWarning(RUQOLA_DDPAPI_LOG) << "empty_callback "<< obj;
    }
}

void create_channel(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    if (!obj.isEmpty()) {
        const QString rid = obj.value(QLatin1String("rid")).toString();
        if (!rid.isEmpty()) {
            account->joinRoom(rid);
        }
        if (account->ruqolaLogger()) {
            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("create Channel :") + QJsonDocument(root).toJson());
        }
    }
}

DDPClient::DDPClient(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , m_uid(1)
    , m_loginJob(0)
    , m_loginStatus(NotConnected)
    , m_connected(false)
    , m_attemptedPasswordLogin(false)
    , m_attemptedTokenLogin(false)
    , mRocketChatMessage(new RocketChatMessage)
    , mRocketChatAccount(account)
{
}

DDPClient::~DDPClient()
{
    mWebSocket->close();
    //Don't delete socket when we use specific socket.
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
    connect(mWebSocket, &AbstractWebSocket::sslErrors, this, &DDPClient::onSslErrors);
}

void DDPClient::start()
{
    if (!mWebSocket) {
        if (!RuqolaTestWebSocket::_k_ruqola_webSocket) {
            mWebSocket = new RuqolaWebSocket(this);
        } else {
            mWebSocket = RuqolaTestWebSocket::_k_ruqola_webSocket;
        }
        initializeWebSocket();
    }
    connect(mRocketChatAccount, &RocketChatAccount::serverUrlChanged, this, &DDPClient::onServerURLChange);

    if (!mUrl.isEmpty()) {
        const QUrl serverUrl = adaptUrl(mUrl);
        if (!serverUrl.isValid()) {
            setLoginStatus(LoginFailed);
        } else {
            mWebSocket->openUrl(serverUrl);
            qCDebug(RUQOLA_DDPAPI_LOG) << "Trying to connect to URL" << serverUrl;
        }
    } else {
        qCDebug(RUQOLA_DDPAPI_LOG) << "url is empty";
    }
}

void DDPClient::setLoginJobId(quint64 jobid)
{
    m_loginJob = jobid;
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
        mWebSocket->openUrl(adaptUrl(mUrl));
        connect(mWebSocket, &AbstractWebSocket::connected, this, &DDPClient::onWSConnected);
        qCDebug(RUQOLA_DDPAPI_LOG) << "Reconnecting" << mUrl;
    }
}

DDPClient::LoginStatus DDPClient::loginStatus() const
{
    return m_loginStatus;
}

void DDPClient::setLoginStatus(DDPClient::LoginStatus l)
{
    qCDebug(RUQOLA_DDPAPI_LOG) << "Setting login status to" << l;
    m_loginStatus = l;
    Q_EMIT loginStatusChanged();

    // reset flags
    if (l == LoginFailed) {
        m_attemptedPasswordLogin = false;
        m_attemptedTokenLogin = false;
    }
}

bool DDPClient::isConnected() const
{
    return m_connected;
}

bool DDPClient::isLoggedIn() const
{
    return m_loginStatus == LoggedIn;
}

QString DDPClient::cachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QQueue<QPair<QString, QJsonDocument> > DDPClient::messageQueue() const
{
    return m_messageQueue;
}

quint64 DDPClient::leaveRoom(const QString &roomID)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->leaveRoom(roomID, m_uid);
    return method(result, empty_callback, DDPClient::Persistent);
}

quint64 DDPClient::hideRoom(const QString &roomID)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->hideRoom(roomID, m_uid);
    return method(result, empty_callback, DDPClient::Persistent);
}

quint64 DDPClient::toggleFavorite(const QString &roomID, bool favorite)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->toggleFavorite(roomID, favorite, m_uid);
    return method(result, empty_callback, DDPClient::Persistent);
}

quint64 DDPClient::createChannel(const QString &name, const QStringList &userList, bool readOnly)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->createChannel(name, userList, readOnly, m_uid);
    return method(result, create_channel, DDPClient::Persistent);
}

quint64 DDPClient::setRoomName(const QString &roomId, const QString &name)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomName(roomId, name, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

quint64 DDPClient::setRoomTopic(const QString &roomId, const QString &topic)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomTopic(roomId, topic, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

quint64 DDPClient::setRoomType(const QString &roomId, bool privateChannel)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomType(roomId, privateChannel, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

quint64 DDPClient::setRoomEncrypted(const QString &roomId, bool encrypted)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomEncrypted(roomId, encrypted, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}


quint64 DDPClient::setRoomDescription(const QString &roomId, const QString &description)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomDescription(roomId, description, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

quint64 DDPClient::archiveRoom(const QString &roomId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->archiveRoom(roomId, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

quint64 DDPClient::setRoomIsReadOnly(const QString &roomId, bool readOnly)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomIsReadOnly(roomId, readOnly, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

quint64 DDPClient::setRoomAnnouncement(const QString &roomId, const QString &announcement)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setRoomAnnouncement(roomId, announcement, m_uid);
    return method(result, change_room_settings, DDPClient::Persistent);
}

quint64 DDPClient::disableNotifications(const QString &roomId, bool disabled)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->disableNotifications(roomId, disabled, m_uid);
    return method(result, change_notifications_settings, DDPClient::Persistent);
}

quint64 DDPClient::hideUnreadStatus(const QString &roomId, bool disabled)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->hideUnreadStatus(roomId, disabled, m_uid);
    return method(result, change_notifications_settings, DDPClient::Persistent);
}

quint64 DDPClient::audioNotifications(const QString &roomId, const QString &value)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->audioNotifications(roomId, value, m_uid);
    return method(result, change_notifications_settings, DDPClient::Persistent);
}

quint64 DDPClient::mobilePushNotifications(const QString &roomId, const QString &value)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->mobilePushNotifications(roomId, value, m_uid);
    return method(result, change_notifications_settings, DDPClient::Persistent);
}

quint64 DDPClient::desktopNotifications(const QString &roomId, const QString &value)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->desktopNotifications(roomId, value, m_uid);
    return method(result, change_notifications_settings, DDPClient::Persistent);
}

quint64 DDPClient::emailNotifications(const QString &roomId, const QString &value)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->emailNotifications(roomId, value, m_uid);
    return method(result, change_notifications_settings, DDPClient::Persistent);
}

quint64 DDPClient::unreadAlert(const QString &roomId, const QString &value)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->unreadAlert(roomId, value, m_uid);
    return method(result, change_notifications_settings, DDPClient::Persistent);
}

void DDPClient::subscribeRoomMessage(const QString &roomId)
{
    QJsonArray params;
    params.append(QJsonValue(roomId));
    subscribe(QStringLiteral("stream-room-messages"), params);

    const QJsonArray params2{
        QJsonValue(QStringLiteral("%1/%2").arg(roomId).arg(QStringLiteral("deleteMessage"))),
                true
    };
    subscribe(QStringLiteral("stream-notify-room"), params2);
    const QJsonArray params3{
        QJsonValue(QStringLiteral("%1/%2").arg(roomId).arg(QStringLiteral("typing"))),
                true
    };
    subscribe(QStringLiteral("stream-notify-room"), params3);
}

quint64 DDPClient::eraseRoom(const QString &roomID)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->eraseRoom(roomID, m_uid);
    return method(result, erase_room, DDPClient::Persistent);
}

quint64 DDPClient::openDirectChannel(const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->openDirectChannel(userId, m_uid);
    return method(result, open_direct_channel, DDPClient::Persistent);
}

quint64 DDPClient::setReaction(const QString &emoji, const QString &messageId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->setReaction(emoji, messageId, m_uid);
    return method(result, empty_callback, DDPClient::Persistent);
}

quint64 DDPClient::createPrivateGroup(const QString &name, const QStringList &userList)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->createPrivateGroup(name, userList, m_uid);
    return method(result, empty_callback, DDPClient::Persistent);
}

quint64 DDPClient::deleteMessage(const QString &messageId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->deleteMessage(messageId, m_uid);
    return method(result, delete_message, DDPClient::Persistent);
}

quint64 DDPClient::deleteFileMessage(const QString &roomId, const QString &fileid, const QString &channelType)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->deleteFileMessage(fileid, m_uid);    

    std::function<void(QJsonObject, RocketChatAccount *)> callback = [ roomId, channelType ](const QJsonObject &root, RocketChatAccount *account) {
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

quint64 DDPClient::listEmojiCustom()
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->listEmojiCustom(m_uid);
    return method(result, list_emoji_custom, DDPClient::Persistent);
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
        std::function<void(QJsonObject,
                           RocketChatAccount *)> callbackUnsubscribeAutoComplete
                = [=](const QJsonObject &root, RocketChatAccount *account) {
            if (account->ruqolaLogger()) {
                account->ruqolaLogger()->
                        dataReceived(QByteArrayLiteral(
                                         "Unsubscribe AutoComplete:") + QJsonDocument(root).toJson());
            } else {
                qDebug() << " Unsubscribe AutoComplete" << root;
                qCDebug(RUQOLA_DDPAPI_LOG) << " Unsubscribe AutoComplete" << root;
            }
        };
        method(resultUnsubscribe, callbackUnsubscribeAutoComplete, DDPClient::Persistent);
    };

    return method(result, callback, DDPClient::Persistent);
}

quint64 DDPClient::channelAndPrivateAutocomplete(const QString &pattern, const QString &exception)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->channelAndPrivateAutocomplete(pattern, exception, m_uid);
    return method(result, channel_and_private_autocomplete, DDPClient::Persistent);
}

quint64 DDPClient::getUsersOfRoom(const QString &roomId, bool showAll)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->getUsersOfRoom(roomId, showAll, m_uid);
    std::function<void(QJsonObject, RocketChatAccount *)> callback = [ roomId ](const QJsonObject &root, RocketChatAccount *account) {
        if (account->ruqolaLogger()) {
            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Get Users of Room:") + QJsonDocument(root).toJson());
        } else {
            qCDebug(RUQOLA_DDPAPI_LOG) << " parse users for room" << roomId;
        }
        account->parseUsersForRooms(roomId, root);
    };

    return method(result, callback, DDPClient::Persistent);
}

quint64 DDPClient::roomFiles(const QString &roomId)
{
    const quint64 subscribeId = m_uid;

    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->roomFiles(roomId, subscribeId);
    std::function<void(QJsonObject, RocketChatAccount *)> callback = [=](const QJsonObject &root, RocketChatAccount *account) {
        if (account->ruqolaLogger()) {
            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Room File done:") + QJsonDocument(root).toJson());
        } else {
            qDebug() << "Room Files " << root;
            qCDebug(RUQOLA_DDPAPI_LOG) << " Room Files" << root;
        }
        account->insertFilesList(roomId);

        const RocketChatMessage::RocketChatMessageResult resultUnsubscribe = mRocketChatMessage->unsubscribe(subscribeId);
        std::function<void(QJsonObject,
                           RocketChatAccount *)> callbackUnsubscribeAutoComplete
                = [=](const QJsonObject &root, RocketChatAccount *account) {
            if (account->ruqolaLogger()) {
                account->ruqolaLogger()->
                        dataReceived(QByteArrayLiteral(
                                         "Unsubscribe room files:") + QJsonDocument(root).toJson());
            } else {
                qDebug()
                        << " Unsubscribe room files" << root;
                qCDebug(RUQOLA_DDPAPI_LOG)
                        << " Unsubscribe room files" << root;
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

quint64 DDPClient::clearUnreadMessages(const QString &roomID)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->readMessages(roomID, m_uid);
    return method(result, empty_callback, DDPClient::Persistent);
}

quint64 DDPClient::inputChannelAutocomplete(const QString &pattern, const QString &exceptions)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->inputChannelAutocomplete(pattern, exceptions,  m_uid);
    return method(result, input_user_channel_autocomplete, DDPClient::Persistent);
}

quint64 DDPClient::inputUserAutocomplete(const QString &pattern, const QString &exceptions)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->inputUserAutocomplete(pattern, exceptions,  m_uid);
    return method(result, input_user_channel_autocomplete, DDPClient::Persistent);
}

quint64 DDPClient::loginProvider(const QString &credentialToken, const QString &credentialSecret)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->loginProvider(credentialToken, credentialSecret, m_uid);
    return method(result, empty_callback, DDPClient::Ephemeral);
}

quint64 DDPClient::login(const QString &username, const QString &password)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->login(username, password, m_uid);
    return method(result, empty_callback, DDPClient::Ephemeral);
}

quint64 DDPClient::addUserToRoom(const QString &username, const QString &roomId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->addUserToRoom(username, roomId, m_uid);
    return method(result, add_user_to_room, DDPClient::Persistent);
}

quint64 DDPClient::starMessage(const QString &messageId, const QString &rid, bool starred)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->starMessage(messageId, rid, starred, m_uid);
    return method(result, star_message, DDPClient::Persistent);
}

quint64 DDPClient::messageSearch(const QString &rid, const QString &pattern)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->messageSearch(rid, pattern, m_uid);
    return method(result, message_search, DDPClient::Persistent);
}

quint64 DDPClient::unBlockUser(const QString &rid, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->unblockUser(rid, userId, m_uid);
    return method(result, unblock_user, DDPClient::Persistent);
}

quint64 DDPClient::blockUser(const QString &rid, const QString &userId)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->blockUser(rid, userId, m_uid);
    return method(result, block_user, DDPClient::Persistent);
}

quint64 DDPClient::informTypingStatus(const QString &roomId, bool typing, const QString &userName)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->informTypingStatus(roomId, userName, typing, m_uid);
    qint64 bytes = mWebSocket->sendTextMessage(result.result);
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

quint64 DDPClient::ignoreUser(const QString &roomId, const QString &userId, bool ignore)
{
    const RocketChatMessage::RocketChatMessageResult result = mRocketChatMessage->ignoreUser(roomId, userId, ignore, m_uid);
    return method(result, user_ignore, DDPClient::Persistent);
}


quint64 DDPClient::method(const RocketChatMessage::RocketChatMessageResult &result, std::function<void(QJsonObject, RocketChatAccount *)> callback, DDPClient::MessageType messageType)
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

quint64 DDPClient::method(const QString &method, const QJsonDocument &params, std::function<void(QJsonObject, RocketChatAccount *)> callback, DDPClient::MessageType messageType)
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

void DDPClient::subscribe(const QString &collection, const QJsonArray &params)
{
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

    qint64 bytes = mWebSocket->sendTextMessage(QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact)));
    if (bytes < json.length()) {
        qCDebug(RUQOLA_DDPAPI_LOG) << "ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << json;
    }
    m_uid++;
}

void DDPClient::onTextMessageReceived(const QString &message)
{
    QJsonDocument response = QJsonDocument::fromJson(message.toUtf8());
    if (!response.isNull() && response.isObject()) {
        QJsonObject root = response.object();

        QString messageType = root.value(QLatin1String("msg")).toString();

        if (messageType == QLatin1String("updated")) {
            //nothing to do.
            qCDebug(RUQOLA_DDPAPI_LOG) << " message updated ! not implemented yet" << response;
        } else if (messageType == QLatin1String("result")) {
            quint64 id = root.value(QLatin1String("id")).toString().toULongLong();

            if (m_callbackHash.contains(id)) {
                std::function<void(QJsonObject, RocketChatAccount *)> callback = m_callbackHash.take(id);

                callback(root, mRocketChatAccount);
            }
            Q_EMIT result(id, QJsonDocument(root.value(QLatin1String("result")).toObject()));

            if (id == m_loginJob) {
                if (root.value(QLatin1String("error")).toObject().value(QLatin1String("error")).toInt() == 403) {
                    qCDebug(RUQOLA_DDPAPI_LOG) << "Wrong password or token expired";

                    login(); // Let's keep trying to log in
                } else {
                    const QString token = root.value(QLatin1String("result")).toObject().value(QLatin1String("token")).toString();
                    mRocketChatAccount->settings()->setAuthToken(token);
                    mRocketChatAccount->restApi()->setAuthToken(token);
                    mRocketChatAccount->restApi()->setUserId(root.value(QLatin1String("id")).toString());
                    setLoginStatus(DDPClient::LoggedIn);
                }
            }
        } else if (messageType == QLatin1String("connected")) {
            //qCDebug(RUQOLA_DDPAPI_LOG) << "Connected";
            qDebug() << "Connected";
            m_connected = true;
            setLoginStatus(DDPClient::LoggingIn);
            Q_EMIT connectedChanged();
        } else if (messageType == QLatin1String("error")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << "ERROR!!" << message;
        } else if (messageType == QLatin1String("ping")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << "Ping - Pong";
            pong();
        } else if (messageType == QLatin1String("added")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << "ADDING element" <<response;
            Q_EMIT added(root);
        } else if (messageType == QLatin1String("changed")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << "Changed element" <<response;
            Q_EMIT changed(root);
        } else if (messageType == QLatin1String("ready")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << "READY element" <<response;
            executeSubsCallBack(root);
        } else if (messageType == QLatin1String("removed")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << "REMOVED element" <<response;
            Q_EMIT removed(root);
        } else if (messageType == QLatin1String("nosub")) {
            qCDebug(RUQOLA_DDPAPI_LOG) << "Unsubscribe element" <<message;
        } else {
            qCDebug(RUQOLA_DDPAPI_LOG) << "received something unhandled:" << message;
        }
    }
}

quint64 DDPClient::loadHistory(const QJsonArray &params)
{
    return method(QStringLiteral("loadHistory"), QJsonDocument(params), process_backlog);
}

void DDPClient::login()
{
    if (!mRocketChatAccount->settings()->password().isEmpty()) {
        // If we have a password and we couldn't log in, let's stop here
        if (m_attemptedPasswordLogin) {
            setLoginStatus(LoginFailed);
            return;
        }
        m_attemptedPasswordLogin = true;

        //m_loginJob = login(mRocketChatAccount->settings()->userName(), mRocketChatAccount->settings()->password());
        mRocketChatAccount->defaultAuthenticationInterface()->login();
    } else if (!mRocketChatAccount->settings()->authToken().isEmpty() && !m_attemptedTokenLogin) {
        m_attemptedPasswordLogin = true;
        QJsonObject json;
        json[QStringLiteral("resume")] = mRocketChatAccount->settings()->authToken();
        m_loginJob = method(QStringLiteral("login"), QJsonDocument(json));
    } else {
        setLoginStatus(LoginFailed);
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
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "onWSConnected: ERROR! I couldn't send all of my message. This is a bug! (try again)";
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << mWebSocket->isValid() << mWebSocket->error() << mWebSocket->requestUrl();
    } else {
        qCDebug(RUQOLA_DDPAPI_COMMAND_LOG) << "Successfully sent " << serialize;
    }
}

void DDPClient::onSslErrors(const QList<QSslError> &errors)
{
    qCDebug(RUQOLA_DDPAPI_LOG) << "SSL error" << errors.count();
    for (const QSslError &err : errors) {
        qCDebug(RUQOLA_DDPAPI_LOG) << "error ssl type:" << err.errorString();
    }
    mWebSocket->ignoreSslErrors();
}

void DDPClient::onWSclosed()
{
    if (mWebSocket->closeCode() != QWebSocketProtocol::CloseCodeNormal) {
        qCDebug(RUQOLA_DDPAPI_LOG) << "WebSocket CLOSED" << mWebSocket->closeReason() << mWebSocket->error() << mWebSocket->closeCode();
    }
    setLoginStatus(NotConnected);
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
