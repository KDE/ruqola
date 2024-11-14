/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "rocketchatbackend.h"
#include "ruqola_authentication_debug.h"
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
#include "connection.h"
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;

void process_updatePublicsettings(const QJsonObject &obj, RocketChatAccount *account)
{
    // qDebug() << " obj " << obj;
    // Update it.
    account->parsePublicSettings(obj, true);

    // qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Update Public Settings:") + QJsonDocument(obj).toJson());
    }
}

void process_publicsettings(const QJsonObject &obj, RocketChatAccount *account)
{
    // qDebug() << " obj " << obj;
    account->parsePublicSettings(obj, false);

    // qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Public Settings:") + QJsonDocument(obj).toJson());
    }
}

void process_publicsettings_administrator(const QJsonObject &obj, RocketChatAccount *account)
{
    Q_EMIT account->publicSettingLoaded(obj);

    // qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Administrator Public Settings:") + QJsonDocument(obj).toJson());
    }
}

void process_privatesettings_administrator(const QJsonObject &obj, RocketChatAccount *account)
{
    Q_EMIT account->privateSettingLoaded(obj);

    // qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Administrator Private Settings:") + QJsonDocument(obj).toJson());
    }
}

void process_permissions_administrator(const QJsonObject &obj, RocketChatAccount *account)
{
    Q_EMIT account->permissionSettingLoaded(obj);

    // qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Administrator Permissions:") + QJsonDocument(obj).toJson());
    }
}

void rooms_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value("result"_L1).toObject();
    RoomModel *model = account->roomModel();

    // qDebug() << " doc " << doc;

    // QJsonArray removed = obj.value("remove"_L1).toArray();
    // qDebug() << " rooms_parsing: room removed *************************************************" << removed;
    const QJsonArray updated = obj.value("update"_L1).toArray();
    // qDebug() << " rooms_parsing: updated  *******************************************************: "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject roomJson = updated.at(i).toObject();
        const QString roomType = roomJson.value("t"_L1).toString();
        if (account->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(roomJson);

            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Rooms:") + d.toJson());
        }
        // TODO: why checking the room type?
        if (roomType == QLatin1Char('c') // Chat
            || roomType == QLatin1Char('p') /*Private chat*/
            || roomType == QLatin1Char('d') /*Direct chat*/) {
            // let's be extra safe around crashes
            if (account->loginStatus() == AuthenticationManager::LoggedIn) {
                model->updateRoom(roomJson);
            }
        }
    }
}

void getsubscription_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    // qCDebug(RUQOLA_MESSAGE_LOG) << " getsubscription_parsing " << root;
    const QJsonObject obj = root.value("result"_L1).toObject();
    RoomModel *model = account->roomModel();

    // qDebug() << " doc " << doc;

    const QJsonArray removed = obj.value("remove"_L1).toArray();
    if (!removed.isEmpty()) {
        // TODO implement it.
        qDebug() << " room removed " << removed;
    }

    const QJsonArray updated = obj.value("update"_L1).toArray();
    // qDebug() << " updated : "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value("t"_L1).toString();
        if (account->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(room);

            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Rooms subscriptions:") + d.toJson());
        }
        if (roomType == QLatin1Char('c') // Chat
            || roomType == QLatin1Char('p') // Private chat
            || roomType == QLatin1Char('d')) { // Direct chat
            // let's be extra safe around crashes
            if (account->loginStatus() == AuthenticationManager::LoggedIn) {
                model->addRoom(room);
            }
        } else if (roomType == QLatin1Char('l')) { // Live chat
            qCDebug(RUQOLA_LOG) << "Live Chat not implemented yet";
        }
    }
    // We need to load all room after get subscription to update parameters
    QJsonObject params;
    // TODO use timeStamp too
    params["$date"_L1] = QJsonValue(0); // get ALL rooms we've ever seen
    // Add timestamp https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-rooms
    account->ddp()->method(QStringLiteral("rooms/get"), params, rooms_parsing);

    account->initializeAccount();
}

RocketChatBackend::RocketChatBackend(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
    connect(mRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RocketChatBackend::slotLoginStatusChanged);
    connect(mRocketChatAccount, &RocketChatAccount::userIdChanged, this, &RocketChatBackend::subscribeRegistration);
    connect(mRocketChatAccount, &RocketChatAccount::changed, this, &RocketChatBackend::slotChanged);
    connect(mRocketChatAccount, &RocketChatAccount::added, this, &RocketChatBackend::slotAdded);
    connect(mRocketChatAccount, &RocketChatAccount::removed, this, &RocketChatBackend::slotRemoved);
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
        if (!Ruqola::self()->useRestApiLogin() || (restApi && restApi->authenticationManager()->isLoggedIn())) {
            ddpLogin();
        }
    }
}

void RocketChatBackend::loadPublicSettings(qint64 timeStamp)
{
    auto ddp = mRocketChatAccount->ddp();
    // https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-public-settings
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
        qDebug() << "RocketChatBackend::loadPublicSettings load from: " << params;
        ddp->method(QStringLiteral("public-settings/get"), params, process_updatePublicsettings);
    } else {
        ddp->method(QStringLiteral("public-settings/get"), params, process_publicsettings);
    }
}

void RocketChatBackend::loadPermissionsAdministrator(qint64 timeStamp)
{
    auto ddp = mRocketChatAccount->ddp();
    if (!ddp->isConnected()) {
        return;
    }
    // https://developer.rocket.chat/docs/get-permissions
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
    }
    qDebug() << " params " << params;
    ddp->method(QStringLiteral("permissions/get"), params, process_permissions_administrator);
}

void RocketChatBackend::loadPrivateSettingsAdministrator(qint64 timeStamp)
{
    auto ddp = mRocketChatAccount->ddp();
    if (!ddp->isConnected()) {
        return;
    }
    // https://developer.rocket.chat/docs/get-permissions
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
    }
    qDebug() << " params " << params;
    ddp->method(QStringLiteral("private-settings/get"), params, process_privatesettings_administrator);
}

void RocketChatBackend::loadPublicSettingsAdministrator(qint64 timeStamp)
{
    auto ddp = mRocketChatAccount->ddp();
    if (!ddp->isConnected()) {
        return;
    }
    // https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-public-settings
    QJsonObject params;
    if (timeStamp != -1) {
        // "params": [ { "$date": 1480377601 } ]
        params["$date"_L1] = timeStamp;
    }
    qDebug() << " params " << params;
    ddp->method(QStringLiteral("public-settings/get"), params, process_publicsettings_administrator);
}

void RocketChatBackend::updateVideoConferenceInfo(const Message &m)
{
    // Update video conf info
    if (m.messageType() == Message::MessageType::VideoConference) {
        if (m.blocks()) {
            const auto blocks{m.blocks()};
            for (const auto &b : blocks->blocks()) {
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
            mRocketChatAccount->deleteMessageFromDatabase(room->displayFName(), messageId);
        }
        // We don't know if we delete a message from thread. So look at in threadModel if we have this identifier
        MessagesModel *threadMessageModel = mRocketChatAccount->threadMessageModel();
        threadMessageModel->deleteMessage(messageId);
    }
}

void RocketChatBackend::addMessagesFromLocalDataBase(const QList<Message> &messages)
{
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
        QJsonObject o = v.toObject();
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(o);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("Message:") + d.toJson());
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
                mRocketChatAccount->addMessageToDataBase(room->displayFName(), m);
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
        qCDebug(RUQOLA_AUTHENTICATION_LOG) << "login ok" << mRocketChatAccount->accountName() << mRocketChatAccount->userName();
        initializeSubscription(ddp);
    } else {
        qCWarning(RUQOLA_AUTHENTICATION_LOG) << "could not reconnect" << mRocketChatAccount->accountName() << mRocketChatAccount->userName();
    }
}

void RocketChatBackend::slotLoginStatusChanged()
{
    if (mRocketChatAccount->loginStatus() == AuthenticationManager::LoggedIn) {
        auto restApi = mRocketChatAccount->restApi();
        if (Ruqola::self()->useRestApiLogin()) {
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
            initializeSubscription(ddp);
        }
        restApi->listAllPermissions();
        restApi->getPrivateSettings();
        restApi->getOwnInfo();
    }
}

void RocketChatBackend::initializeSubscription(DDPClient *ddp)
{
    QJsonObject params;
    // TODO use timeStamp too
    params["$date"_L1] = QJsonValue(0); // get ALL rooms we've ever seen

    std::function<void(QJsonObject, RocketChatAccount *)> subscription_callback = [=](const QJsonObject &obj, RocketChatAccount *account) {
        getsubscription_parsing(obj, account);
    };

    ddp->method(QStringLiteral("subscriptions/get"), params, subscription_callback);
}

void RocketChatBackend::slotPrivateInfoDone(const QJsonObject &data)
{
    qDebug() << "parse private info not implemented . Needed ? "; // << data;
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
    if (mRocketChatAccount->serverVersion().isEmpty()) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "Can't autologin yet, waiting for parseServerVersionDone";
        return;
    }
    if (mRocketChatAccount->authMethodType() == AuthenticationManager::AuthMethodType::Password) {
        if (mRocketChatAccount->password().isEmpty()) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "Can't autologin (yet?), no password set";
            return;
        }
    } else if (mRocketChatAccount->authMethodType() == AuthenticationManager::AuthMethodType::PersonalAccessToken) {
        if (mRocketChatAccount->authToken().isEmpty()) {
            qCDebug(RUQOLA_RECONNECT_LOG) << "Can't autologin (yet?), no authToken";
            return;
        }
    } else {
        qCWarning(RUQOLA_RECONNECT_LOG) << " tryAutoLogin check not implemented for " << mRocketChatAccount->authMethodType();
    }
    if (Ruqola::self()->useRestApiLogin()) {
        qCDebug(RUQOLA_RECONNECT_LOG) << "try login REST API" << mRocketChatAccount->accountName();
        mRocketChatAccount->tryLogin();
    } else {
        qCDebug(RUQOLA_RECONNECT_LOG) << "try login ddp" << mRocketChatAccount->accountName();
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
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("users: Removed user:") + d.toJson());
        } else {
            qDebug() << "USER REMOVED VALUE" << object;
        }
    } else if (collection == "stream-notify-logged"_L1) {
        qDebug() << "removed stream-notify-logged " << object;
    } else {
        qDebug() << " RocketChatBackend::slotRemove " << collection << " object " << object;
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << " Other collection type  removed " << collection << " object " << object;
    }
}

void RocketChatBackend::slotAdded(const QJsonObject &object)
{
    const QString collection = object.value("collection"_L1).toString();
    // qDebug() << " void RocketChatBackend::slotAdded(const QJsonObject &object)" << object;
    if (collection == "stream-room-messages"_L1) {
        qCDebug(RUQOLA_LOG) << mRocketChatAccount->accountName() << ":stream-room-messages : " << object;
    } else if (collection == "users"_L1) {
        const QJsonObject fields = object.value("fields"_L1).toObject();
        const QString username = fields.value("username"_L1).toString();
        if (username == mRocketChatAccount->settings()->userName()) {
            mRocketChatAccount->settings()->setUserId(object["id"_L1].toString().toLatin1());
            qCDebug(RUQOLA_LOG) << "User id set to " << mRocketChatAccount->settings()->userId();
        } else {
            // TODO add current user ? me ?
            User user;
            user.parseUser(object);
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("users: Add User:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "USER ADDED VALUE" << object;
            }
            mRocketChatAccount->usersModel()->addUser(user);
        }
        qCDebug(RUQOLA_LOG) << "NEW USER ADDED: " << username << fields;
    } else if (collection == "rooms"_L1) {
        qCDebug(RUQOLA_LOG) << "NEW ROOMS ADDED: " << object;
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
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("autocompleteRecords: Add User:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "AutoCompleteRecords VALUE" << object;
        }
    } else if (collection == "room_files"_L1) {
        qWarning() << " Necessary to implement room_files !!!!!";
        /*
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("room_files: Add Files:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "room_files VALUE" << object;
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
        qCDebug(RUQOLA_LOG) << " RocketChatBackend::slotChanged stream-room-messages " << contents;
        processIncomingMessages(contents, false);
    } else if (collection == "users"_L1) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("users: User Changed:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "USER CHANGED" << object;
        }
    } else if (collection == "rooms"_L1) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("rooms: Room Changed:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "ROOMS CHANGED: " << object;
        }
    } else if (collection == "stream-notify-user"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << "account name: " << mRocketChatAccount->accountName() << " contents " << contents
                            << fields.value("args"_L1).toArray().toVariantList();

        if (eventname.endsWith("/subscriptions-changed"_L1)) {
            RoomModel *model = mRocketChatAccount->roomModel();
            model->updateSubscription(contents);
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(fields);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: subscriptions-changed:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "stream-notify-user: subscriptions-changed " << object;
            }
        } else if (eventname.endsWith("/rooms-changed"_L1)) {
            RoomModel *model = mRocketChatAccount->roomModel();
            // qDebug() << " EVENT " << eventname << " contents " << contents << fields.value("args"_L1).toArray().toVariantList();
            const QString actionName = contents[0].toString();
            if (actionName == "updated"_L1) {
                qCDebug(RUQOLA_LOG) << " Update room " << contents;
                const QJsonObject roomData = contents[1].toObject();
                model->updateRoom(roomData);
                mRocketChatAccount->updateUserInRoom(roomData);
            } else if (actionName == "inserted"_L1) {
                qCDebug(RUQOLA_LOG) << "****************************************** insert new Room !!!!!" << contents;
                const QJsonObject roomData = contents[1].toObject();
                model->insertRoom(roomData);
                mRocketChatAccount->playNewRoomNotification();
            } else if (actionName == "removed"_L1) {
                qCDebug(RUQOLA_LOG) << "Remove channel" << contents;
                const QJsonObject roomData = contents[1].toObject();
                // TODO use rid
                model->removeRoom(QByteArray());
            } else {
                qWarning() << "rooms-changed invalid actionName " << actionName;
            }
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: Room Changed:") + d.toJson());
            }
        } else if (eventname.endsWith("/notification"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: notification:") + d.toJson());
            } else {
                qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "NOTIFICATION: " << object;
            }
            mRocketChatAccount->sendNotification(contents);
        } else if (eventname.endsWith("/webrtc"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: webrtc: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "WEBRTC CHANGED: " << object;
            }
            qCWarning(RUQOLA_LOG) << "stream-notify-user : WEBRTC ? " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/otr"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: otr: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "OTR CHANGED: " << object;
            }
            mRocketChatAccount->parseOtr(contents);
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-notify-user : OTR ? " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/message"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: message: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "stream-notify-user : Message: " << object;
            }
            const QJsonObject roomData = contents[0].toObject();
            mRocketChatAccount->addMessage(roomData);
            qCDebug(RUQOLA_LOG) << "stream-notify-user : Message  " << eventname << " contents " << contents;
            qDebug() << "stream-notify-user : Message  " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/userData"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: userData event: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "stream-notify-user: userData event: " << object;
            }
            mRocketChatAccount->updateUserData(contents);
            // TODO update avatar
            qCDebug(RUQOLA_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;

        } else if (eventname.endsWith("/video-conference"_L1)) {
            qDebug() << " *******************************************************************" << eventname << " contents " << contents;
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: video-conference ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "stream-notify-user: video-conference :account name:" << mRocketChatAccount->accountName() << "object" << object;
            }
            mRocketChatAccount->parseVideoConference(contents);
            qCDebug(RUQOLA_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;
        } else if (eventname.endsWith("/force_logout"_L1)) {
            qCDebug(RUQOLA_LOG) << " FORCE LOGOUT!!!!";
            // Clear auth token otherwise we can't reconnect.
            mRocketChatAccount->setAuthToken({});
            qCDebug(RUQOLA_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;
        } else {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: Unknown event: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "Unknown change: " << object;
            }
            qCDebug(RUQOLA_LOG) << "stream-notify-user : message event " << eventname << " contents " << contents;
        }
    } else if (collection == "stream-notify-room"_L1) {
        qCDebug(RUQOLA_LOG) << " stream-notify-room " << collection << " object " << object;
        const QString eventname = fields.value("eventName"_L1).toString();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value("args"_L1).toArray().toVariantList();

        if (eventname.endsWith("/deleteMessage"_L1)) {
            // qDebug() << " deleteMessage :" << object;
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-room: DeleteMessage:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "Delete message" << object;
            }
            // Move code in rocketChatAccount ?

            QString roomId = eventname;
            roomId.remove(QStringLiteral("/deleteMessage"));
            MessagesModel *messageModel = mRocketChatAccount->messageModelForRoom(roomId.toLatin1());
            if (messageModel) {
                const QByteArray messageId = contents.at(0).toObject()["_id"_L1].toString().toLatin1();
                messageModel->deleteMessage(messageId);
                Room *room = mRocketChatAccount->room(roomId.toLatin1());
                if (room) {
                    mRocketChatAccount->deleteMessageFromDatabase(room->displayFName(), messageId);
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
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-room: typing:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "typing message" << object;
            }

            QString roomId = eventname;
            roomId.remove(QStringLiteral("/user-activity"));
            // TODO Perhaps not necessary to convert to variantlist. Need to investigate
            // qCWarning(RUQOLA_LOG) << "stream-notify-room:  typing event ? " << eventname << " content  " << contents.toVariantList();
            const QString typingUserName = contents.toVariantList().at(0).toString();
            if (typingUserName != mRocketChatAccount->settings()->userName()) {
                const bool status = contents.toVariantList().at(1).toBool();
                mRocketChatAccount->receiveTypingNotificationManager()->insertTypingNotification(roomId.toLatin1(), typingUserName, status);
            }
        } else if (eventname.endsWith("/deleteMessageBulk"_L1)) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("deleteMessageBulk: DeleteMessage:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "Delete message" << object;
            }
            qDebug() << " DELETE MESSAGE Bulk not IMPLEMENTED yet";
            QString roomId = eventname;
            roomId.remove(QStringLiteral("/deleteMessageBulk"));
            qDebug() << "UNIMPLEMENT!!!!!! deleteMessageBulk " << collection << " object " << object;
            // QJsonObject({"collection":"stream-notify-room","fields":{"args":[{"excludePinned":false,"ignoreDiscussion":true,"rid":"QgCf8GcnXYW5QXiHN","ts":{"$gt":{"$date":946681200000},"$lt":{"$date":1599602400000}},"users":[]}],"eventName":"QgCf8GcnXYW5QXiHN/deleteMessageBulk"},"id":"id","msg":"changed"})
        } else {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-room:  Unknown event ?") + d.toJson());
            } else {
                qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-notify-room:  Unknown event ? " << eventname;
            }
        }
    } else if (collection == "stream-notify-logged"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value("args"_L1).toArray().toVariantList();
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
        if (eventname == "deleteCustomSound"_L1) {
            mRocketChatAccount->deleteCustomSound(contents);
        } else if (eventname == "updateCustomSound"_L1) {
            mRocketChatAccount->updateCustomSound(contents);
        } else {
            qDebug() << " NEED TO IMPLEMENT stream-notify-all " << object;
        }
        //{"collection":"stream-notify-all","fields":{"args":[{"soundData":{"_id":"LmShBQiqaCJDbgduR","_updatedAt":{"$date":1603350386481},"extension":"mp3","name":"ss"}}],"eventName":"deleteCustomSound"},"id":"id","msg":"changed"}
    } else if (collection == "stream-stdout"_L1) {
        mRocketChatAccount->addStdoutInfo(contents);
    } else if (collection == "stream-roles"_L1) {
        const QString eventname = fields.value("eventName"_L1).toString();
        if (eventname == "roles"_L1) {
            mRocketChatAccount->updateRoles(contents);
        }
    } else {
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << " Other collection type changed " << collection << " object " << object;
    }
}

void RocketChatBackend::subscribeRegistration()
{
    qCDebug(RUQOLA_LOG) << "subscribe registration";
    // TODO verify if we don"t send two subscription.
    const QString userId{QString::fromLatin1(mRocketChatAccount->settings()->userId())};
    {
        // Subscribe notification.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("notification"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe room-changed.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("rooms-changed"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe subscriptions-changed
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("subscriptions-changed"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("message"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("otr"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("webrtc"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("video-conference"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe avatardata
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("userData"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("banners"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        // Subscribe force_logout
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("force_logout"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }

    //    { // Verify as in RC we don't have it
    //        const QJsonArray params{QJsonValue(QStringLiteral("video-conference"))};
    //        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    //    }
    // Sound
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteCustomSound"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateCustomSound"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteEmojiCustom"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("public-settings-changed"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    { // Verify it
        const QJsonArray params{QJsonValue(QStringLiteral("permissions-changed"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    { // Verify it
        const QJsonArray params{QJsonValue(QStringLiteral("license"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    { // Verify it
        const QJsonArray params{QJsonValue(QStringLiteral("public-info"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateEmojiCustom"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteEmojiCustom"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("roles-change"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }

    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateAvatar"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("Users:NameChanged"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("Users:Deleted"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("banner-changed"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteCustomUserStatus"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateCustomUserStatus"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("voip.statuschanged"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("user-status"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("permissions-changed"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("private-settings-changed"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("roles"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-roles"), params);
    }
}

#include "moc_rocketchatbackend.cpp"
