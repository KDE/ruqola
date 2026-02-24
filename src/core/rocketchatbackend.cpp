/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "rocketchatbackend.h"
#include "ruqola_authentication_debug.h"
#include "ruqola_backend_debug.h"
#include "ruqola_reconnect_core_debug.h"

#include "authenticationmanager/ddpauthenticationmanager.h"
#include "config-ruqola.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "model/messagesmodel.h"
#include "model/roommodel.h"
#include "model/usersmodel.h"
#include "receivetypingnotificationmanager.h"
#include "rocketchataccount.h"
#include "rocketchataccountsettings.h"
#include "ruqola.h"
#include "ruqola_debug.h"
#include "ruqola_message_debug.h"
#include "ruqola_unknown_collectiontype_debug.h"
#include "ruqolalogger.h"
#include "videoconference/videoconferencemessageinfomanager.h"

#include "authenticationmanager/restauthenticationmanager.h"
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;

RocketChatBackend::RocketChatBackend(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
    connect(mRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RocketChatBackend::slotLoginStatusChanged);
    connect(mRocketChatAccount, &RocketChatAccount::userIdChanged, this, &RocketChatBackend::subscribeRegistration);
    connect(mRocketChatAccount, &RocketChatAccount::ddpConnectedChanged, this, &RocketChatBackend::slotDDPConnectedChanged);
    connect(mRocketChatAccount, &RocketChatAccount::passwordAvailable, this, &RocketChatBackend::tryAutoLogin);
}

RocketChatBackend::~RocketChatBackend() = default;

void RocketChatBackend::loadServerInfo()
{
    auto restApi = mRocketChatAccount->restApi();

    restApi->serverInfo();
    connect(restApi, &Connection::serverInfoDone, this, &RocketChatBackend::parseServerVersionDone, Qt::UniqueConnection);
    connect(restApi, &Connection::privateInfoDone, this, &RocketChatBackend::slotPrivateInfoDone, Qt::UniqueConnection);
}

void RocketChatBackend::slotDDPConnectedChanged(bool connected)
{
    if (connected) {
        // This ends up calling loadPublicSettings() below
        mRocketChatAccount->loadAccountSettings();

        // In RestAPI-login mode (the default), we can only DDP-login if we have an auth token from rest api
        auto restApi = mRocketChatAccount->restApi();
        if (!Ruqola::useRestApiLogin() || (restApi && restApi->authenticationManager()->isLoggedIn())) {
            ddpLogin();
        }
    }
}

void RocketChatBackend::updateVideoConferenceInfo(const Message &m)
{
    // Update video conf info
    if (m.messageType() == Message::MessageType::VideoConference) {
        if (m.blocks()) {
            const auto blocks{m.blocks()};
            const auto blocksElements = blocks->blocks();
            for (const auto &b : blocksElements) {
                if (!b.callId().isEmpty()) {
                    mRocketChatAccount->videoConferenceMessageInfoManager()->addCallId(b.callId());
                }
            }
        }
    }
}

void RocketChatBackend::removeMessageFromLocalDatabase(const QList<QByteArray> &messageIds, const QByteArray &roomId)
{
    if (messageIds.isEmpty()) {
        return;
    }
    auto messageModel = mRocketChatAccount->messageModelForRoom(roomId);
    for (const auto &messageId : messageIds) {
        messageModel->deleteMessage(messageId);
        Room *room = mRocketChatAccount->room(roomId);
        if (room) {
            mRocketChatAccount->deleteMessageFromDatabase(room->roomId(), messageId);
        }
        // We don't know if we delete a message from thread. So look at in threadModel if we have this identifier
        MessagesModel *threadMessageModel = mRocketChatAccount->threadMessageModel();
        threadMessageModel->deleteMessage(messageId);
    }
}

void RocketChatBackend::addMessagesSyncAfterLoadingFromDatabase(const QList<Message> &messages)
{
    qCWarning(RUQOLA_MESSAGE_LOG) << " addMessagesSyncAfterLoadingFromDatabase :" << messages.count();

    if (messages.isEmpty()) {
        return;
    }
    MessagesModel *messageModel = nullptr;
    for (const auto &message : messages) {
        if (!messageModel) {
            const QByteArray roomId = message.roomId();
            messageModel = mRocketChatAccount->messageModelForRoom(roomId);
        }
        updateVideoConferenceInfo(message);
        if (messageModel) {
            if (!message.threadMessageId().isEmpty()) {
                mRocketChatAccount->updateThreadMessageList(message);
                // qDebug() << " Update thread message";
            }
        } else {
            qCWarning(RUQOLA_MESSAGE_LOG) << " MessageModel is empty for :" << message.roomId() << " It's a bug for sure.";
        }
    }
    if (messageModel) {
        messageModel->addMessagesSyncAfterLoadingFromDatabase(messages);
    }
}

void RocketChatBackend::addMessagesFromLocalDataBase(const QList<Message> &messages)
{
    qCWarning(RUQOLA_MESSAGE_LOG) << " addMessagesFromLocalDataBase :" << messages.count();

    if (messages.isEmpty()) {
        return;
    }
    MessagesModel *messageModel = nullptr;
    for (const auto &message : messages) {
        if (!messageModel) {
            const QByteArray roomId = message.roomId();
            messageModel = mRocketChatAccount->messageModelForRoom(roomId);
        }
        updateVideoConferenceInfo(message);
        if (messageModel) {
            if (!message.threadMessageId().isEmpty()) {
                mRocketChatAccount->updateThreadMessageList(message);
                // qDebug() << " Update thread message";
            }
        } else {
            qCWarning(RUQOLA_MESSAGE_LOG) << " MessageModel is empty for :" << message.roomId() << " It's a bug for sure.";
        }
    }
    if (messageModel) {
        messageModel->addMessages(messages);
    }
}

void RocketChatBackend::processIncomingMessages(const QJsonArray &messages, bool loadHistory, bool restApi)
{
    QHash<MessagesModel *, QList<Message>> dispatcher;
    QByteArray lastRoomId;
    MessagesModel *messageModel = nullptr;
    Room *room = nullptr;
    for (const QJsonValue &v : messages) {
        const QJsonObject o = v.toObject();
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(o);
            mRocketChatAccount->ruqolaLogger()->dataReceived("Message:"_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_MESSAGE_LOG) << " new message: " << o;
        }
        Message m;
        m.parseMessage(o, restApi, mRocketChatAccount->emojiManager());
        updateVideoConferenceInfo(m);
        const QByteArray roomId = m.roomId();
        if (roomId != lastRoomId) {
            messageModel = mRocketChatAccount->messageModelForRoom(roomId);
            room = mRocketChatAccount->room(roomId);
            lastRoomId = roomId;
        }
        if (messageModel) {
            if (!m.threadMessageId().isEmpty()) {
                mRocketChatAccount->updateThreadMessageList(m);
                // qDebug() << " Update thread message";
            }
            if (room) {
                mRocketChatAccount->addMessageToDataBase(room->roomId(), m);
                if (!loadHistory) {
                    room->newMessageAdded();
                }
            }
            dispatcher[messageModel].append(std::move(m));
        } else {
            qCWarning(RUQOLA_MESSAGE_LOG) << " MessageModel is empty for :" << m.roomId() << " It's a bug for sure.";
        }
    }
    for (auto it = dispatcher.cbegin(); it != dispatcher.cend(); ++it) {
        it.key()->addMessages(it.value());
    }
}

void RocketChatBackend::ddpLogin()
{
    auto restApi = mRocketChatAccount->restApi();
    auto ddp = mRocketChatAccount->ddp();
    ddp->setServerUrl(restApi->serverUrl());
    ddp->authenticationManager()->setAuthToken(restApi->authenticationManager()->authToken());
    // if (ddp->authenticationManager()->loginPassword(mRocketChatAccount->settings()->userName(), mRocketChatAccount->settings()->password())) {
    if (ddp->authenticationManager()->login()) {
        qCDebug(RUQOLA_AUTHENTICATION_LOG) << "ddpLogin: login ok" << mRocketChatAccount->accountName() << mRocketChatAccount->userName();
        ddp->initializeSubscription();
    } else {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << "ddpLogin: could not reconnect" << mRocketChatAccount->accountName() << mRocketChatAccount->userName();
    }
}

void RocketChatBackend::slotLoginStatusChanged()
{
    if (mRocketChatAccount->loginStatus() == AuthenticationManager::LoggedIn) {
        auto restApi = mRocketChatAccount->restApi();
        if (Ruqola::useRestApiLogin()) {
            mRocketChatAccount->settings()->setAuthToken(restApi->authenticationManager()->authToken());
            mRocketChatAccount->settings()->setExpireToken(restApi->authenticationManager()->tokenExpires());

            connect(restApi, &Connection::getOwnInfoDone, mRocketChatAccount, &RocketChatAccount::parseOwnInfoDone, Qt::UniqueConnection);
            restApi->setAuthToken(restApi->authenticationManager()->authToken());
            restApi->setUserId(restApi->authenticationManager()->userId());

            if (mRocketChatAccount->ddp()->isConnected()) {
                ddpLogin();
            }
        } else {
            // Now that we are logged in the ddp authentication manager has all the information we need
            mRocketChatAccount->settings()->setAuthToken(mRocketChatAccount->ddp()->authenticationManager()->authToken());
            mRocketChatAccount->settings()->setExpireToken(mRocketChatAccount->ddp()->authenticationManager()->tokenExpires());
            restApi->setAuthToken(mRocketChatAccount->ddp()->authenticationManager()->authToken());
            restApi->setUserId(mRocketChatAccount->ddp()->authenticationManager()->userId());

            connect(restApi, &Connection::getOwnInfoDone, mRocketChatAccount, &RocketChatAccount::parseOwnInfoDone, Qt::UniqueConnection);

            auto ddp = mRocketChatAccount->ddp();
#if 0
            const qint64 timeStamp = -1;
#else
            const qint64 timeStamp = mRocketChatAccount->globalRoomsTimeStamp();
#endif
            ddp->initializeSubscription(timeStamp);
        }
        // TODO how we do in offline mode ?
        restApi->listAllPermissions();
        restApi->getPrivateSettings();
        restApi->getOwnInfo();
    }
}

void RocketChatBackend::slotPrivateInfoDone([[maybe_unused]] const QJsonObject &data)
{
    qCDebug(RUQOLA_BACKEND_LOG) << "parse private info not implemented . Needed ? "; // << data;
}

void RocketChatBackend::parseServerVersionDone(const QString &version)
{
    qCDebug(RUQOLA_LOG) << "parseServerVersionDone" << version;
    mRocketChatAccount->setServerVersion(version);
    tryAutoLogin();
}

void RocketChatBackend::tryAutoLogin()
{
    qCDebug(RUQOLA_RECONNECT_LOG) << "tryAutoLogin for account" << mRocketChatAccount->accountName();
    if (mRocketChatAccount->ruqolaServerConfig()->serverVersion().isEmpty()) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "tryAutoLogin: can't autologin yet, waiting for parseServerVersionDone";
        return;
    }
    if (mRocketChatAccount->settings()->authMethodType() == AuthenticationManager::AuthMethodType::Password) {
        if (mRocketChatAccount->settings()->password().isEmpty()) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "tryAutoLogin: can't autologin (yet?), no password set";
            return;
        }
    } else if (mRocketChatAccount->settings()->authMethodType() == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
        if (mRocketChatAccount->settings()->authToken().isEmpty()) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "tryAutoLogin: can't autologin (yet?), no authToken";
            return;
        }
    } else {
        qCWarning(RUQOLA_RECONNECT_LOG) << " tryAutoLogin: check not implemented for " << mRocketChatAccount->settings()->authMethodType();
    }
    if (Ruqola::useRestApiLogin()) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "tryAutoLogin: try login REST API" << mRocketChatAccount->accountName();
        mRocketChatAccount->tryLogin();
    } else {
        qCDebug(RUQOLA_RECONNECT_LOG) << "tryAutoLogin: try login ddp" << mRocketChatAccount->accountName();
        mRocketChatAccount->ddp()->login();
    }
}

void RocketChatBackend::slotRemoved(const QJsonObject &object)
{
    const QString collection = object.value("collection"_L1).toString();
    if (collection == "users"_L1) {
        const QByteArray id = object.value("id"_L1).toString().toLatin1();
        mRocketChatAccount->usersModel()->removeUser(id);
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("users: Removed user:"_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_BACKEND_LOG) << "USER REMOVED VALUE" << object;
        }
    } else if (collection == "stream-notify-logged"_L1) {
        qCDebug(RUQOLA_BACKEND_LOG) << "removed stream-notify-logged " << object;
    } else {
        qCDebug(RUQOLA_BACKEND_LOG) << " RocketChatBackend::slotRemove " << collection << " object " << object;
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << " Other collection type  removed " << collection << " object " << object;
    }
}

void RocketChatBackend::slotAdded(const QJsonObject &object)
{
    const QString collection = object.value("collection"_L1).toString();
    // qDebug() << " void RocketChatBackend::slotAdded(const QJsonObject &object)" << object;
    if (collection == "stream-room-messages"_L1) {
        qCDebug(RUQOLA_BACKEND_LOG) << mRocketChatAccount->accountName() << ":stream-room-messages : " << object;
    } else if (collection == "users"_L1) {
        const QJsonObject fields = object.value("fields"_L1).toObject();
        const QString username = fields.value("username"_L1).toString();
        if (username == mRocketChatAccount->settings()->userName()) {
            mRocketChatAccount->settings()->setUserId(object["id"_L1].toString().toLatin1());
            qCDebug(RUQOLA_BACKEND_LOG) << "User id set to " << mRocketChatAccount->settings()->userId();
        } else {
            // TODO add current user ? me ?
            User user;
            user.parseUser(object);
            // qDebug() << " SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << object;
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("users: Add User:"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "USER ADDED VALUE" << object;
            }
            mRocketChatAccount->usersModel()->addUser(user);
        }
        qCDebug(RUQOLA_BACKEND_LOG) << "NEW USER ADDED: " << username << fields;
    } else if (collection == "rooms"_L1) {
        qCDebug(RUQOLA_BACKEND_LOG) << "NEW ROOMS ADDED: " << object;
    } else if (collection == "stream-notify-user"_L1) {
        // qDebug() << "stream-notify-user: " << object;
    } else if (collection == "stream-notify-all"_L1) {
        // void RocketChatBackend::slotChanged(const QJsonObject &object)
        // QJsonObject({"collection":"stream-notify-all","fields":{"args":[{"soundData":{"_id":"oikq5aYewRkYBGebK","_updatedAt":{"$date":1603350166714},"extension":"mp3","name":"test"}}],"eventName":"deleteCustomSound"},"id":"id","msg":"changed"})

        // qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ":stream-notify-all: " << object;
        // TODO verify that all is ok !
    } else if (collection == "autocompleteRecords"_L1) {
        qWarning() << "autocompleteRecords: Necessary to implement autocompleteRecords !!!!!";
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("autocompleteRecords: Add User:"_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_BACKEND_LOG) << "AutoCompleteRecords VALUE" << object;
        }
    } else if (collection == "room_files"_L1) {
        qWarning() << " Necessary to implement room_files !!!!!";
        /*
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("room_files: Add Files:"_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_BACKEND_LOG) << "room_files VALUE" << object;
        }
        File file;
        file.parseFile(object, false);
        mFiles.append(std::move(file));
        */
    } else if (collection == "stream-notify-room"_L1) {
        // qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ":stream-notify-room not implemented: "<< object;
    } else if (collection == "stream-notify-logged"_L1) {
        // qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ": stream-notify-logged not implemented: "<< object;
    } else {
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ":Unknown added element: " << object;
    }
}

void RocketChatBackend::slotChanged(const QJsonObject &object)
{
    const QString collection = object["collection"_L1].toString();
    const QJsonObject fields = object.value("fields"_L1).toObject();
    const QJsonArray contents = fields.value("args"_L1).toArray();

    if (collection == "stream-room-messages"_L1) {
        qCDebug(RUQOLA_BACKEND_LOG) << " RocketChatBackend::slotChanged stream-room-messages " << contents;
        processIncomingMessages(contents, false);
    } else if (collection == "users"_L1) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("users: User Changed:"_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_BACKEND_LOG) << "USER CHANGED" << object;
        }
    } else if (collection == "rooms"_L1) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("rooms: Room Changed:"_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_BACKEND_LOG) << "ROOMS CHANGED: " << object;
        }
    } else if (collection == "stream-notify-user"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        qCDebug(RUQOLA_BACKEND_LOG) << " EVENT " << eventname << "account name: " << mRocketChatAccount->accountName() << " contents " << contents
                                    << fields.value("args"_L1).toArray().toVariantList();

        if (eventname.endsWith("/uiInteraction"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(fields);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: uiInteraction:"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user: uiInteraction " << object;
            }
            // TODO implement it
            qCDebug(RUQOLA_BACKEND_LOG) << "uiInteraction********* " << contents;
            if (mRocketChatAccount) {
                Q_EMIT mRocketChatAccount->showUiInteraction(contents);
            }
        } else if (eventname.endsWith("/subscriptions-changed"_L1)) {
            RoomModel *model = mRocketChatAccount->roomModel();
            // TODO fix database
            model->updateSubscription(contents);
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(fields);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: subscriptions-changed:"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user: subscriptions-changed " << object;
            }
        } else if (eventname.endsWith("/rooms-changed"_L1)) {
            RoomModel *model = mRocketChatAccount->roomModel();
            // qDebug() << " EVENT " << eventname << " contents " << contents << fields.value("args"_L1).toArray().toVariantList();
            const QString actionName = contents[0].toString();
            if (actionName == "updated"_L1) {
                qCDebug(RUQOLA_LOG) << " Update room " << contents;
                const QJsonObject roomData = contents[1].toObject();
                const QByteArray roomId = model->updateRoom(roomData);
                mRocketChatAccount->updateRoomInDatabase(roomId);
                mRocketChatAccount->updateUserInRoom(roomData);
                // qDebug() << " rooms-changed***************************************************************************";
            } else if (actionName == "inserted"_L1) {
                // qDebug() << " inserted***************************************************************************";
                qCDebug(RUQOLA_BACKEND_LOG) << "****************************************** insert new Room !!!!!" << contents;
                const QJsonObject roomData = contents[1].toObject();
                const QByteArray roomId = model->insertRoom(roomData);
                if (auto r = model->findRoom(roomId); r) {
                    mRocketChatAccount->addRoomToDataBase(r);
                }
                mRocketChatAccount->playNewRoomNotification();
            } else if (actionName == "removed"_L1) {
                qCDebug(RUQOLA_BACKEND_LOG) << "Remove channel" << contents;
                const QJsonObject roomData = contents[1].toObject();
                // qDebug() << "roomData " << roomData;
                Q_ASSERT(false);
                model->removeRoom(QByteArray());
            } else {
                qWarning() << "rooms-changed invalid actionName " << actionName;
            }
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: Room Changed:"_ba + d.toJson());
            }
        } else if (eventname.endsWith("/notification"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: notification:"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "NOTIFICATION: " << object;
            }
            mRocketChatAccount->sendNotification(contents);
        } else if (eventname.endsWith("/webrtc"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: webrtc: "_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "WEBRTC CHANGED: " << object;
            }
            qCWarning(RUQOLA_LOG) << "stream-notify-user : WEBRTC ? " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/otr"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: otr: "_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "OTR CHANGED: " << object;
            }
            mRocketChatAccount->parseOtr(contents);
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-notify-user : OTR ? " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/message"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: message: "_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user : Message: " << object;
            }
            const QJsonObject roomData = contents[0].toObject();
            mRocketChatAccount->addMessage(roomData);
            qCDebug(RUQOLA_LOG) << "stream-notify-user : Message  " << eventname << " contents " << contents;
            // qDebug() << "stream-notify-user : Message  " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/userData"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: userData event: "_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user: userData event: " << object;
            }
            mRocketChatAccount->updateUserData(contents);
            // TODO update avatar
            qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;

        } else if (eventname.endsWith("/video-conference"_L1)) {
            // qDebug() << " *******************************************************************" << eventname << " contents " << contents;
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: video-conference "_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user: video-conference :account name:" << mRocketChatAccount->accountName() << "object" << object;
            }
            mRocketChatAccount->parseVideoConference(contents);
            qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/force_logout"_L1)) {
            qCDebug(RUQOLA_BACKEND_LOG) << " FORCE LOGOUT!!!!";
            // Clear auth token otherwise we can't reconnect.
            mRocketChatAccount->settings()->setAuthToken({});
            qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;
        } else {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-user: Unknown event: "_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "Unknown change: " << object;
            }
            qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;
        }
    } else if (collection == "stream-notify-room"_L1) {
        qCDebug(RUQOLA_BACKEND_LOG) << " stream-notify-room " << collection << " object " << object;
        const QString eventname = fields.value("eventName"_L1).toString();
        qCDebug(RUQOLA_BACKEND_LOG) << " EVENT " << eventname << " contents " << contents << fields.value("args"_L1).toArray().toVariantList();

        if (eventname.endsWith("/deleteMessage"_L1)) {
            // qDebug() << " deleteMessage :" << object;
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-room: DeleteMessage:"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "Delete message" << object;
            }
            // Move code in rocketChatAccount ?

            QString roomId = eventname;
            roomId.remove(u"/deleteMessage"_s);
            MessagesModel *messageModel = mRocketChatAccount->messageModelForRoom(roomId.toLatin1());
            if (messageModel) {
                const QByteArray messageId = contents.at(0).toObject()["_id"_L1].toString().toLatin1();
                messageModel->deleteMessage(messageId);
                const Room *room = mRocketChatAccount->room(roomId.toLatin1());
                if (room) {
                    mRocketChatAccount->deleteMessageFromDatabase(room->roomId(), messageId);
                }
                // We don't know if we delete a message from thread. So look at in threadModel if we have this identifier
                MessagesModel *threadMessageModel = mRocketChatAccount->threadMessageModel();
                threadMessageModel->deleteMessage(messageId);
            } else {
                qCWarning(RUQOLA_MESSAGE_LOG) << " MessageModel is empty for :" << roomId << " It's a bug for sure.";
            }
        } else if (eventname.endsWith("/user-activity"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-room: typing:"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "typing message" << object;
            }

            QString roomId = eventname;
            roomId.remove(u"/user-activity"_s);
            // TODO Perhaps not necessary to convert to variantlist. Need to investigate
            qCDebug(RUQOLA_BACKEND_LOG) << "stream-notify-room: typing event ? " << eventname << " content  " << contents.toVariantList() << " object "
                                        << object;
            const QString typingUserName = contents.toVariantList().at(0).toString();
            if (typingUserName != mRocketChatAccount->settings()->userName()) {
                const bool status = Utils::userActivity(contents);
                mRocketChatAccount->receiveTypingNotificationManager()->insertTypingNotification(roomId.toLatin1(), typingUserName, status);
            }
        } else if (eventname.endsWith("/deleteMessageBulk"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("deleteMessageBulk: DeleteMessage:"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_BACKEND_LOG) << "Delete message" << object;
            }
            qCDebug(RUQOLA_BACKEND_LOG) << " DELETE MESSAGE Bulk not IMPLEMENTED yet";
            QString roomId = eventname;
            roomId.remove(u"/deleteMessageBulk"_s);
            qCDebug(RUQOLA_BACKEND_LOG) << "UNIMPLEMENT!!!!!! deleteMessageBulk " << collection << " object " << object;
            // QJsonObject({"collection":"stream-notify-room","fields":{"args":[{"excludePinned":false,"ignoreDiscussion":true,"rid":"QgCf8GcnXYW5QXiHN","ts":{"$gt":{"$date":946681200000},"$lt":{"$date":1599602400000}},"users":[]}],"eventName":"QgCf8GcnXYW5QXiHN/deleteMessageBulk"},"id":"id","msg":"changed"})
        } else {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-room:  Unknown event ?"_ba + d.toJson());
            } else {
                qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-notify-room:  Unknown event ? " << eventname;
            }
        }
    } else if (collection == "stream-notify-logged"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value("args"_L1).toArray().toVariantList();
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-logged: "_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-notify-logged: " << eventname;
        }
        if (eventname == "roles-change"_L1) {
            mRocketChatAccount->rolesChanged(contents);
        } else if (eventname == "updateAvatar"_L1) {
            mRocketChatAccount->avatarChanged(contents);
        } else if (eventname == "updateEmojiCustom"_L1) {
            mRocketChatAccount->addUpdateEmojiCustomList(contents);
        } else if (eventname == "Users:NameChanged"_L1) {
            mRocketChatAccount->setNameChanged(contents);
        } else if (eventname == "Users:Deleted"_L1) {
            mRocketChatAccount->deleteUser(contents);
        } else if (eventname == "deleteCustomUserStatus"_L1) {
            mRocketChatAccount->deleteCustomUserStatus(contents);
        } else if (eventname == "updateCustomUserStatus"_L1) {
            mRocketChatAccount->updateCustomUserStatus(contents);
        } else if (eventname == "user-status"_L1) {
            mRocketChatAccount->setUserStatusChanged(contents);
        } else if (eventname == "deleteEmojiCustom"_L1) {
            mRocketChatAccount->deleteEmojiCustom(contents);
        } else if (eventname == "permissions-changed"_L1) {
            mRocketChatAccount->permissionUpdated(contents);
        } else if (eventname == "private-settings-changed"_L1) {
            mRocketChatAccount->privateSettingsUpdated(contents);
        } else {
            qWarning() << "stream-notify-logged not supported " << fields;
        }
    } else if (collection == "stream-notify-all"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("stream-notify-all: "_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-notify-all: " << eventname;
        }

        if (eventname == "deleteCustomSound"_L1) {
            mRocketChatAccount->deleteCustomSound(contents);
        } else if (eventname == "updateCustomSound"_L1) {
            mRocketChatAccount->updateCustomSound(contents);
        } else {
            qCDebug(RUQOLA_BACKEND_LOG) << " NEED TO IMPLEMENT stream-notify-all " << object;
        }
        //{"collection":"stream-notify-all","fields":{"args":[{"soundData":{"_id":"LmShBQiqaCJDbgduR","_updatedAt":{"$date":1603350386481},"extension":"mp3","name":"ss"}}],"eventName":"deleteCustomSound"},"id":"id","msg":"changed"}
    } else if (collection == "stream-stdout"_L1) {
        mRocketChatAccount->addStdoutInfo(contents);
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("stream-stdout: "_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-stdout: " << contents;
        }
    } else if (collection == "stream-roles"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("stream-roles: "_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-roles: " << contents;
        }
        if (eventname == "roles"_L1) {
            mRocketChatAccount->updateRoles(contents);
        }
    } else if (collection == "stream-apps"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived("stream-apps: "_ba + d.toJson());
        } else {
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-apps: " << eventname;
        }
        if (eventname == "apps"_L1) {
            mRocketChatAccount->updateApps(contents);
        }
    } else {
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << " Other collection type changed " << collection << " object " << object;
    }
}

void RocketChatBackend::subscribeRegistration()
{
    qCDebug(RUQOLA_LOG) << "subscribe registration";
    const QString userId{QString::fromLatin1(mRocketChatAccount->settings()->userId())};
    const QStringList listStreamNotifierUser{
        u"notification"_s,
        u"rooms-changed"_s,
        u"subscriptions-changed"_s,
        u"message"_s,
        u"otr"_s,
        u"webrtc"_s,
        u"video-conference"_s,
        u"userData"_s,
        u"banners"_s,
        u"force_logout"_s,
        u"uiInteraction"_s,
    };
    for (const QString &str : listStreamNotifierUser) {
        QJsonArray params;
        params.append(QJsonValue(u"%1/%2"_s.arg(userId, str)));
        mRocketChatAccount->ddp()->subscribe(u"stream-notify-user"_s, params);
    }

    const QStringList listStreamNotifierAll{
        u"deleteCustomSound"_s,
        u"updateCustomSound"_s,
        u"deleteEmojiCustom"_s,
        u"public-settings-changed"_s,
        u"permissions-changed"_s,
        u"license"_s,
        u"public-info"_s,
        u"userData"_s,
        u"banners"_s,
        u"force_logout"_s,
    };
    for (const QString &str : listStreamNotifierAll) {
        QJsonArray params;
        params.append(QJsonValue(str));
        mRocketChatAccount->ddp()->subscribe(u"stream-notify-all"_s, params);
    }

    const QStringList listStreamNotifierLogged{
        u"updateEmojiCustom"_s,
        u"deleteEmojiCustom"_s,
        u"roles-change"_s,
        u"updateAvatar"_s,
        u"Users:NameChanged"_s,
        u"Users:Deleted"_s,
        u"banner-changed"_s,
        u"deleteCustomUserStatus"_s,
        u"updateCustomUserStatus"_s,
        u"voip.statuschanged"_s,
        u"user-status"_s,
        u"permissions-changed"_s,
        u"private-settings-changed"_s,
        u"omnichannel.priority-changed"_s,
    };
    for (const QString &str : listStreamNotifierLogged) {
        QJsonArray params;
        params.append(QJsonValue(str));
        mRocketChatAccount->ddp()->subscribe(u"stream-notify-logged"_s, params);
    }

    {
        const QJsonArray params{QJsonValue(u"roles"_s)};
        mRocketChatAccount->ddp()->subscribe(u"stream-roles"_s, params);
    }

    {
        const QJsonArray params{QJsonValue(u"apps"_s)};
        mRocketChatAccount->ddp()->subscribe(u"stream-apps"_s, params);
    }
    {
        const QJsonArray params{QJsonValue(u"apps-engine"_s)};
        mRocketChatAccount->ddp()->subscribe(u"stream-apps-engine"_s, params);
    }
}

#include "moc_rocketchatbackend.cpp"
