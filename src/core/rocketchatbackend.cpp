/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 * SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "rocketchatbackend.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "model/messagemodel.h"
#include "model/usersmodel.h"
#include "receivetypingnotificationmanager.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "ruqola_message_debug.h"
#include "ruqola_unknown_collectiontype_debug.h"
#include "ruqolalogger.h"
#include "videoconference/videoconferencemessageinfomanager.h"

#include <QJsonArray>

void process_publicsettings(const QJsonObject &obj, RocketChatAccount *account)
{
    // qDebug() << " obj " << obj;
    account->parsePublicSettings(obj);

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

void rooms_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    RoomModel *model = account->roomModel();

    // qDebug() << " doc " << doc;

    // QJsonArray removed = obj.value(QLatin1String("remove")).toArray();
    // qDebug() << " rooms_parsing: room removed *************************************************" << removed;
    const QJsonArray updated = obj.value(QLatin1String("update")).toArray();
    // qDebug() << " rooms_parsing: updated  *******************************************************: "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject roomJson = updated.at(i).toObject();
        const QString roomType = roomJson.value(QLatin1String("t")).toString();
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
            if (account->loginStatus() == DDPAuthenticationManager::LoggedIn) {
                model->updateRoom(roomJson);
            }
        }
    }
}

void getsubscription_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    RoomModel *model = account->roomModel();

    // qDebug() << " doc " << doc;

    const QJsonArray removed = obj.value(QLatin1String("remove")).toArray();
    if (!removed.isEmpty()) {
        // TODO implement it.
        qDebug() << " room removed " << removed;
    }

    const QJsonArray updated = obj.value(QLatin1String("update")).toArray();
    // qDebug() << " updated : "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value(QLatin1String("t")).toString();
        if (account->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(room);

            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Rooms subscriptions:") + d.toJson());
        }
        if (roomType == QLatin1Char('c') // Chat
            || roomType == QLatin1Char('p') // Private chat
            || roomType == QLatin1Char('d')) { // Direct chat
            // let's be extra safe around crashes
            if (account->loginStatus() == DDPAuthenticationManager::LoggedIn) {
                model->addRoom(room);
            }
        } else if (roomType == QLatin1Char('l')) { // Live chat
            qCDebug(RUQOLA_LOG) << "Live Chat not implemented yet";
        }
    }
    // We need to load all room after get subscription to update parameters
    QJsonObject params;
    // TODO use timeStamp too
    params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen
    // Add timestamp https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-rooms
    account->ddp()->method(QStringLiteral("rooms/get"), QJsonDocument(params), rooms_parsing);

    account->initializeAccount();
}

RocketChatBackend::RocketChatBackend(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
    connect(mRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RocketChatBackend::slotLoginStatusChanged);
    connect(mRocketChatAccount, &RocketChatAccount::userIdChanged, this, &RocketChatBackend::slotUserIDChanged);
    connect(mRocketChatAccount, &RocketChatAccount::changed, this, &RocketChatBackend::slotChanged);
    connect(mRocketChatAccount, &RocketChatAccount::added, this, &RocketChatBackend::slotAdded);
    connect(mRocketChatAccount, &RocketChatAccount::removed, this, &RocketChatBackend::slotRemoved);
    connect(mRocketChatAccount, &RocketChatAccount::connectedChanged, this, &RocketChatBackend::slotConnectedChanged);
    connect(mRocketChatAccount, &RocketChatAccount::passwordChanged, this, &RocketChatBackend::tryAutoLogin);
}

RocketChatBackend::~RocketChatBackend() = default;

// TODO load public info even if we are not connected.
void RocketChatBackend::slotConnectedChanged()
{
    auto ddp = mRocketChatAccount->ddp();
    if (!ddp->isConnected()) {
        return;
    }

    auto restApi = mRocketChatAccount->restApi();

    restApi->serverInfo(false);
    connect(restApi, &RocketChatRestApi::Connection::serverInfoDone, this, &RocketChatBackend::parseServerVersionDone, Qt::UniqueConnection);
    connect(restApi, &RocketChatRestApi::Connection::serverInfoFailed, this, &RocketChatBackend::slotGetServerInfoFailed, Qt::UniqueConnection);
    connect(restApi, &RocketChatRestApi::Connection::privateInfoDone, this, &RocketChatBackend::slotPrivateInfoDone, Qt::UniqueConnection);

    // TODO add timestamp https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-public-settings
    ddp->method(QStringLiteral("public-settings/get"), QJsonDocument(), process_publicsettings);
}

void RocketChatBackend::loadPublicSettingsAdministrator()
{
    auto ddp = mRocketChatAccount->ddp();
    if (!ddp->isConnected()) {
        return;
    }
    // TODO add timestamp https://developer.rocket.chat/reference/api/realtime-api/method-calls/get-public-settings
    ddp->method(QStringLiteral("public-settings/get"), QJsonDocument(), process_publicsettings_administrator);
}

void RocketChatBackend::slotGetServerInfoFailed(bool useDeprecatedVersion)
{
    if (!useDeprecatedVersion) {
        mRocketChatAccount->restApi()->serverInfo(true);
    }
}

void RocketChatBackend::updateVideoConferenceInfo(const Message &m)
{
    // Update video conf info
    if (m.messageType() == Message::MessageType::VideoConference) {
        const auto blocks{m.blocks()};
        for (const auto &b : blocks) {
            if (!b.callId().isEmpty()) {
                mRocketChatAccount->videoConferenceMessageInfoManager()->addCallId(b.callId());
            }
        }
    }
}

void RocketChatBackend::removeMessageFromLocalDatabase(const QVector<Message> &messages, const QString &roomId)
{
    if (messages.isEmpty()) {
        return;
    }
    auto messageModel = mRocketChatAccount->messageModelForRoom(roomId);
    for (const auto &message : messages) {
        const QString messageId{message.messageId()};
        messageModel->deleteMessage(messageId);
        Room *room = mRocketChatAccount->room(roomId);
        if (room) {
            mRocketChatAccount->deleteMessageFromDatabase(room->displayFName(), messageId);
        }
        // We don't know if we delete a message from thread. So look at in threadModel if we have this identifier
        MessageModel *threadMessageModel = mRocketChatAccount->threadMessageModel();
        threadMessageModel->deleteMessage(messageId);
    }
}

void RocketChatBackend::addMessageFromLocalDataBase(const QVector<Message> &messages)
{
    if (messages.isEmpty()) {
        return;
    }
    MessageModel *messageModel = nullptr;
    for (const auto &message : messages) {
        if (!messageModel) {
            const QString roomId = message.roomId();
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
    QHash<MessageModel *, QVector<Message>> dispatcher;
    QString lastRoomId;
    MessageModel *messageModel = nullptr;
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
        Message m(mRocketChatAccount->emojiManager());
        m.parseMessage(o, restApi);
        updateVideoConferenceInfo(m);
        const QString roomId = m.roomId();
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

void RocketChatBackend::slotLoginStatusChanged()
{
    if (mRocketChatAccount->loginStatus() == DDPAuthenticationManager::LoggedIn) {
        // Now that we are logged in the ddp authentication manager has all the information we need
        auto restApi = mRocketChatAccount->restApi();
        mRocketChatAccount->settings()->setAuthToken(mRocketChatAccount->ddp()->authenticationManager()->authToken());
        mRocketChatAccount->settings()->setExpireToken(mRocketChatAccount->ddp()->authenticationManager()->tokenExpires());
        restApi->setAuthToken(mRocketChatAccount->ddp()->authenticationManager()->authToken());
        restApi->setUserId(mRocketChatAccount->ddp()->authenticationManager()->userId());

        connect(restApi, &RocketChatRestApi::Connection::getOwnInfoDone, mRocketChatAccount, &RocketChatAccount::parseOwnInfoDone, Qt::UniqueConnection);
        QJsonObject params;
        // TODO use timeStamp too
        params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen

        std::function<void(QJsonObject, RocketChatAccount *)> subscription_callback = [=](const QJsonObject &obj, RocketChatAccount *account) {
            getsubscription_parsing(obj, account);
        };

        auto ddp = mRocketChatAccount->ddp();
        ddp->method(QStringLiteral("subscriptions/get"), QJsonDocument(params), subscription_callback);

        restApi->listAllPermissions();
        restApi->getPrivateSettings();
        restApi->getOwnInfo();
    }
}

void RocketChatBackend::slotPrivateInfoDone(const QJsonObject &data)
{
    qDebug() << "parse private info not implemented . Needed ? "; // << data;
}

void RocketChatBackend::parseServerVersionDone(const QString &version)
{
    // qCDebug(RUQOLA_LOG) << " void RocketChatBackend::parseServerVersionDone(const QString &version)******************" << version;
    mRocketChatAccount->setServerVersion(version);
    tryAutoLogin();
}

void RocketChatBackend::tryAutoLogin()
{
    if (mRocketChatAccount->serverVersion().isEmpty() || mRocketChatAccount->password().isEmpty()) {
        return;
    }
    mRocketChatAccount->ddp()->login();
}

QVector<File> RocketChatBackend::files() const
{
    return mFiles;
}

QVector<User> RocketChatBackend::users() const
{
    return mUsers;
}

void RocketChatBackend::slotRemoved(const QJsonObject &object)
{
    const QString collection = object.value(QLatin1String("collection")).toString();
    if (collection == QLatin1String("users")) {
        const QString id = object.value(QLatin1String("id")).toString();
        mRocketChatAccount->usersModel()->removeUser(id);
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("users: Removed user:") + d.toJson());
        } else {
            qDebug() << "USER REMOVED VALUE" << object;
        }
    } else if (collection == QLatin1String("stream-notify-logged")) {
        qDebug() << "removed stream-notify-logged " << object;
    } else {
        qDebug() << " RocketChatBackend::slotRemove " << collection << " object " << object;
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << " Other collection type  removed " << collection << " object " << object;
    }
}

void RocketChatBackend::slotAdded(const QJsonObject &object)
{
    const QString collection = object.value(QLatin1String("collection")).toString();
    // qDebug() << " void RocketChatBackend::slotAdded(const QJsonObject &object)" << object;
    if (collection == QLatin1String("stream-room-messages")) {
        qCDebug(RUQOLA_LOG) << mRocketChatAccount->accountName() << ":stream-room-messages : " << object;
    } else if (collection == QLatin1String("users")) {
        const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString username = fields.value(QLatin1String("username")).toString();
        if (username == mRocketChatAccount->settings()->userName()) {
            mRocketChatAccount->settings()->setUserId(object[QStringLiteral("id")].toString());
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
    } else if (collection == QLatin1String("rooms")) {
        qCDebug(RUQOLA_LOG) << "NEW ROOMS ADDED: " << object;
    } else if (collection == QLatin1String("stream-notify-user")) {
        // qDebug() << "stream-notify-user: " << object;
    } else if (collection == QLatin1String("stream-notify-all")) {
        // void RocketChatBackend::slotChanged(const QJsonObject &object)
        // QJsonObject({"collection":"stream-notify-all","fields":{"args":[{"soundData":{"_id":"oikq5aYewRkYBGebK","_updatedAt":{"$date":1603350166714},"extension":"mp3","name":"test"}}],"eventName":"deleteCustomSound"},"id":"id","msg":"changed"})

        // qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ":stream-notify-all: " << object;
        // TODO verify that all is ok !
    } else if (collection == QLatin1String("autocompleteRecords")) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("autocompleteRecords: Add User:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "AutoCompleteRecords VALUE" << object;
        }
        User user;
        user.parseUser(object);
        mUsers.append(std::move(user));
    } else if (collection == QLatin1String("room_files")) {
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
    } else if (collection == QLatin1String("stream-notify-room")) {
        // qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ":stream-notify-room not implemented: "<< object;
    } else if (collection == QLatin1String("stream-notify-logged")) {
        // qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ": stream-notify-logged not implemented: "<< object;
    } else {
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << mRocketChatAccount->accountName() << ":Unknown added element: " << object;
    }
}

void RocketChatBackend::slotChanged(const QJsonObject &object)
{
    const QString collection = object[QStringLiteral("collection")].toString();
    const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
    const QJsonArray contents = fields.value(QLatin1String("args")).toArray();

    if (collection == QLatin1String("stream-room-messages")) {
        qDebug() << " RocketChatBackend::slotChanged stream-room-messages " << contents;
        processIncomingMessages(contents, false);
    } else if (collection == QLatin1String("users")) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("users: User Changed:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "USER CHANGED" << object;
        }
    } else if (collection == QLatin1String("rooms")) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("rooms: Room Changed:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "ROOMS CHANGED: " << object;
        }
    } else if (collection == QLatin1String("stream-notify-user")) {
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << "account name: " << mRocketChatAccount->accountName() << " contents " << contents
                            << fields.value(QLatin1String("args")).toArray().toVariantList();

        if (eventname.endsWith(QLatin1String("/subscriptions-changed"))) {
            RoomModel *model = mRocketChatAccount->roomModel();
            model->updateSubscription(contents);
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(fields);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: subscriptions-changed:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "stream-notify-user: subscriptions-changed " << object;
            }
        } else if (eventname.endsWith(QLatin1String("/rooms-changed"))) {
            RoomModel *model = mRocketChatAccount->roomModel();
            // qDebug() << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();
            const QString actionName = contents[0].toString();
            if (actionName == QLatin1String("updated")) {
                qCDebug(RUQOLA_LOG) << " Update room " << contents;
                const QJsonObject roomData = contents[1].toObject();
                model->updateRoom(roomData);
                mRocketChatAccount->updateUserInRoom(roomData);
            } else if (actionName == QLatin1String("inserted")) {
                qCDebug(RUQOLA_LOG) << "****************************************** insert new Room !!!!!" << contents;
                const QJsonObject roomData = contents[1].toObject();
                model->insertRoom(roomData);
            } else if (actionName == QLatin1String("removed")) {
                qCDebug(RUQOLA_LOG) << "Remove channel" << contents;
                const QJsonObject roomData = contents[1].toObject();
                // TODO use rid
                model->removeRoom(QString());
            } else {
                qWarning() << "rooms-changed invalid actionName " << actionName;
            }
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: Room Changed:") + d.toJson());
            }
        } else if (eventname.endsWith(QLatin1String("/notification"))) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: notification:") + d.toJson());
            } else {
                qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "NOTIFICATION: " << object;
            }
            mRocketChatAccount->sendNotification(contents);
        } else if (eventname.endsWith(QLatin1String("/webrtc"))) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: webrtc: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "WEBRTC CHANGED: " << object;
            }
            qCWarning(RUQOLA_LOG) << "stream-notify-user : WEBRTC ? " << eventname << " contents " << contents;
        } else if (eventname.endsWith(QLatin1String("/otr"))) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: otr: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "OTR CHANGED: " << object;
            }
            mRocketChatAccount->parseOtr(contents);
            qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << "stream-notify-user : OTR ? " << eventname << " contents " << contents;
        } else if (eventname.endsWith(QLatin1String("/message"))) {
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
        } else if (eventname.endsWith(QLatin1String("/userData"))) {
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

        } else if (eventname.endsWith(QLatin1String("/video-conference"))) {
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
    } else if (collection == QLatin1String("stream-notify-room")) {
        qCDebug(RUQOLA_LOG) << " stream-notify-room " << collection << " object " << object;
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();

        if (eventname.endsWith(QLatin1String("/deleteMessage"))) {
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
            MessageModel *messageModel = mRocketChatAccount->messageModelForRoom(roomId);
            if (messageModel) {
                const QString messageId = contents.at(0).toObject()[QStringLiteral("_id")].toString();
                messageModel->deleteMessage(messageId);
                Room *room = mRocketChatAccount->room(roomId);
                if (room) {
                    mRocketChatAccount->deleteMessageFromDatabase(room->displayFName(), messageId);
                }
                // We don't know if we delete a message from thread. So look at in threadModel if we have this identifier
                MessageModel *threadMessageModel = mRocketChatAccount->threadMessageModel();
                threadMessageModel->deleteMessage(messageId);
            } else {
                qCWarning(RUQOLA_MESSAGE_LOG) << " MessageModel is empty for :" << roomId << " It's a bug for sure.";
            }
        } else if (eventname.endsWith(QLatin1String("/user-activity"))) {
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
                mRocketChatAccount->receiveTypingNotificationManager()->insertTypingNotification(roomId, typingUserName, status);
            }
        } else if (eventname.endsWith(QLatin1String("/deleteMessageBulk"))) {
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
            qDebug() << " deleteMessageBulk " << collection << " object " << object;
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
    } else if (collection == QLatin1String("stream-notify-logged")) {
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();
        if (eventname == QLatin1String("roles-change")) {
            mRocketChatAccount->rolesChanged(contents);
        } else if (eventname == QLatin1String("updateAvatar")) {
            mRocketChatAccount->avatarChanged(contents);
        } else if (eventname == QLatin1String("updateEmojiCustom")) {
            mRocketChatAccount->addUpdateEmojiCustomList(contents);
        } else if (eventname == QLatin1String("Users:NameChanged")) {
            mRocketChatAccount->setNameChanged(contents);
        } else if (eventname == QLatin1String("Users:Deleted")) {
            mRocketChatAccount->deleteUser(contents);
        } else if (eventname == QLatin1String("deleteCustomUserStatus")) {
            mRocketChatAccount->deleteCustomUserStatus(contents);
        } else if (eventname == QLatin1String("updateCustomUserStatus")) {
            mRocketChatAccount->updateCustomUserStatus(contents);
        } else if (eventname == QLatin1String("user-status")) {
            mRocketChatAccount->setUserStatusChanged(contents);
        } else if (eventname == QLatin1String("deleteEmojiCustom")) {
            mRocketChatAccount->deleteEmojiCustom(contents);
        } else if (eventname == QLatin1String("permissions-changed")) {
            mRocketChatAccount->permissionUpdated(contents);
        } else if (eventname == QLatin1String("private-settings-changed")) {
            mRocketChatAccount->privateSettingsUpdated(contents);
        } else {
            qWarning() << "stream-notify-logged not supported " << fields;
        }
    } else if (collection == QLatin1String("stream-notify-all")) {
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        if (eventname == QLatin1String("deleteCustomSound")) {
            mRocketChatAccount->deleteCustomSound(contents);
        } else if (eventname == QLatin1String("updateCustomSound")) {
            mRocketChatAccount->updateCustomSound(contents);
        } else {
            qDebug() << " NEED TO IMPLEMENT stream-notify-all " << object;
        }
        //{"collection":"stream-notify-all","fields":{"args":[{"soundData":{"_id":"LmShBQiqaCJDbgduR","_updatedAt":{"$date":1603350386481},"extension":"mp3","name":"ss"}}],"eventName":"deleteCustomSound"},"id":"id","msg":"changed"}
    } else if (collection == QLatin1String("stream-stdout")) {
        // const QString eventname = fields.value(QLatin1String("eventName")).toString();
        mRocketChatAccount->addStdoutInfo(contents);
    } else if (collection == QLatin1String("stream-roles")) {
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        if (eventname == QLatin1String("roles")) {
            mRocketChatAccount->updateRoles(contents);
        }
    } else {
        qCDebug(RUQOLA_UNKNOWN_COLLECTIONTYPE_LOG) << " Other collection type changed " << collection << " object " << object;
    }
}

void RocketChatBackend::clearFilesList()
{
    mFiles.clear();
}

void RocketChatBackend::clearUsersList()
{
    mUsers.clear();
}

void RocketChatBackend::slotUserIDChanged()
{
    // TODO verify if we don"t send two subscription.
    const QString userId{mRocketChatAccount->settings()->userId()};
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
        // Subscribe users in room ? //TODO verify it.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("webrtc"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-room-users"), params);
    }
    // stream-notify-all
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateAvatar"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    { // Verify as in RC we don't have it
        const QJsonArray params{QJsonValue(QStringLiteral("roles-change"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
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
        const QJsonArray params{QJsonValue(QStringLiteral("updateEmojiCustom"))};
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
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteCustomUserStatus"))};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateCustomUserStatus"))};
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
