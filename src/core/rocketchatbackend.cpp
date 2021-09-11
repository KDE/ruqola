/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2018-2021 Laurent Montel <montel@kde.org>
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

#include "rocketchatbackend.h"
#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "model/messagemodel.h"
#include "model/usercompletermodel.h"
#include "model/usersmodel.h"
#include "receivetypingnotificationmanager.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"
#include "ruqola_message_debug.h"
#include "ruqola_unknown_collectiontype_debug.h"
#include "ruqolaglobalconfig.h"
#include "ruqolalogger.h"

#include <QJsonArray>

void process_publicsettings(const QJsonObject &obj, RocketChatAccount *account)
{
    account->parsePublicSettings(obj);

    // qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Public Settings:") + QJsonDocument(obj).toJson());
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
        qDebug() << " room removed " << removed;
    }
    // TODO implement it.

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
            || roomType == QLatin1Char('p') /*Private chat*/
            || roomType == QLatin1Char('d')) { // Direct chat) {
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
    params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen
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
}

RocketChatBackend::~RocketChatBackend()
{
}

// TODO load public info even if we are not connected.
void RocketChatBackend::slotConnectedChanged()
{
    if (!mRocketChatAccount->ddp()->isConnected()) {
        return;
    }

    auto restApi = mRocketChatAccount->restApi();
    auto ddp = mRocketChatAccount->ddp();

    restApi->serverInfo(false);
    connect(restApi, &RocketChatRestApi::Connection::serverInfoDone, this, &RocketChatBackend::parseServerVersionDone, Qt::UniqueConnection);
    connect(restApi, &RocketChatRestApi::Connection::serverInfoFailed, this, &RocketChatBackend::slotGetServerInfoFailed, Qt::UniqueConnection);

    ddp->method(QStringLiteral("public-settings/get"), QJsonDocument(), process_publicsettings);
}

void RocketChatBackend::slotGetServerInfoFailed(bool useDeprecatedVersion)
{
    if (!useDeprecatedVersion) {
        mRocketChatAccount->restApi()->serverInfo(true);
    }
}

void RocketChatBackend::processIncomingMessages(const QJsonArray &messages, bool loadHistory, bool restApi)
{
    QHash<MessageModel *, QVector<Message>> dispatcher;
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
        // qDebug() << " roomId"<<m.roomId() << " add message " << m;
        if (MessageModel *messageModel = mRocketChatAccount->messageModelForRoom(m.roomId())) {
            if (!m.threadMessageId().isEmpty()) {
                mRocketChatAccount->updateThreadMessageList(m);
            }
            dispatcher[messageModel].append(std::move(m));
            if (!loadHistory) {
                if (Room *room = mRocketChatAccount->room(m.roomId())) {
                    room->newMessageAdded();
                }
            }
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

void RocketChatBackend::parseServerVersionDone(const QString &version)
{
    // qCDebug(RUQOLA_LOG) << " void RocketChatBackend::parseServerVersionDone(const QString &version)******************" << version;
    mRocketChatAccount->setServerVersion(version);
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
        mUsers.append(user);
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
        mFiles.append(file);
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

    if (collection == QLatin1String("stream-room-messages")) {
        const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QJsonArray contents = fields.value(QLatin1String("args")).toArray();
        processIncomingMessages(contents, false);
    } else if (collection == QLatin1String("users")) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("users: User Changed:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "USER CHANGED" << object;
        }
        if (mRocketChatAccount->hasOldSubscriptionSupport()) {
            mRocketChatAccount->updateUser(object);
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
        QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        const QJsonArray contents = fields.value(QLatin1String("args")).toArray();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();

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
            const QJsonArray lst = fields.value(QLatin1String("args")).toArray();
            const QString actionName = lst[0].toString();
            if (actionName == QLatin1String("updated")) {
                qCDebug(RUQOLA_LOG) << " Update room " << lst;
                const QJsonObject roomData = lst[1].toObject();
                model->updateRoom(roomData);
            } else if (actionName == QLatin1String("inserted")) {
                qCDebug(RUQOLA_LOG) << "****************************************** insert new Room !!!!!" << lst;
                const QJsonObject roomData = lst[1].toObject();
                model->insertRoom(roomData);
            } else if (actionName == QLatin1String("removed")) {
                qCDebug(RUQOLA_LOG) << "Remove channel" << lst;
                const QJsonObject roomData = lst[1].toObject();
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
            const QJsonArray lst = fields.value(QLatin1String("args")).toArray();
            const QJsonObject roomData = lst[0].toObject();
            mRocketChatAccount->addMessage(roomData);
            qCDebug(RUQOLA_LOG) << "stream-notify-user : Message  " << eventname << " contents " << contents;
            qDebug() << "stream-notify-user : Message  " << eventname << " contents " << contents;
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
        QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        const QJsonArray contents = fields.value(QLatin1String("args")).toArray();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();

        if (eventname.endsWith(QLatin1String("/deleteMessage"))) {
            qDebug() << " if (eventname.endsWith(QLatin1String(/deleteMessage))) {" << object;
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
                // We don't know if we delete a message from thread. So look at in threadModel if we have this identifier
                MessageModel *threadMessageModel = mRocketChatAccount->threadMessageModel();
                threadMessageModel->deleteMessage(messageId);
            } else {
                qCWarning(RUQOLA_MESSAGE_LOG) << " MessageModel is empty for :" << roomId << " It's a bug for sure.";
            }
        } else if (eventname.endsWith(QLatin1String("/typing"))) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-room: typing:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "typing message" << object;
            }

            QString roomId = eventname;
            roomId.remove(QStringLiteral("/typing"));
            // TODO Perhaps not necessary to convert to variantlist. Need to investigate
            // qCWarning(RUQOLA_LOG) << "stream-notify-room:  typing event ? " << eventname << " content  " << contents.toVariantList();
            const QString typingUserName = contents.toVariantList().at(0).toString();
            if (typingUserName != mRocketChatAccount->settings()->userName()) {
                const bool status = contents.toVariantList().at(1).toBool();
                mRocketChatAccount->receiveTypingNotificationManager()->insertTypingNotification(roomId, typingUserName, status);
            }
        } else if (eventname.endsWith(QLatin1String("/deleteMessageBulk"))) {
            qDebug() << " DELETE MESSAGE TO IMPLEMENT";
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
        QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        const QJsonArray contents = fields.value(QLatin1String("args")).toArray();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();
        if (eventname == QLatin1String("roles-change")) {
            mRocketChatAccount->rolesChanged(contents);
        } else if (eventname == QLatin1String("updateAvatar")) {
            mRocketChatAccount->avatarChanged(contents);
        } else if (eventname == QLatin1String("updateEmojiCustom")) {
            qCWarning(RUQOLA_LOG) << "Need to implement updateEmojiCustom :" << fields;
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
        } else {
            qWarning() << "stream-notify-logged not supported " << fields;
        }
    } else if (collection == QLatin1String("stream-notify-all")) {
        QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        if (eventname == QLatin1String("deleteCustomSound")) {
            // TODO
            qDebug() << " NEED TO IMPLEMENT stream-notify-all deleteCustomSound " << object;
        } else if (eventname == QLatin1String("updateCustomSound")) {
            qDebug() << " NEED TO IMPLEMENT stream-notify-all updateCustomSound " << object;
        } else {
            qDebug() << " NEED TO IMPLEMENT stream-notify-all " << object;
        }
        //{"collection":"stream-notify-all","fields":{"args":[{"soundData":{"_id":"LmShBQiqaCJDbgduR","_updatedAt":{"$date":1603350386481},"extension":"mp3","name":"ss"}}],"eventName":"deleteCustomSound"},"id":"id","msg":"changed"}
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
        if (mRocketChatAccount->hasOldSubscriptionSupport()) {
            // Subscribe activeUsers
            QJsonArray params;
            params.append(QJsonValue(params));
            mRocketChatAccount->ddp()->subscribe(QStringLiteral("activeUsers"), params);
        }
    }
    {
        // Subscribe users in room ? //TODO verify it.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("webrtc"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-room-users"), params);
    }
    // stream-notify-all
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateAvatar")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("roles-change")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    // Sound
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteCustomSound")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateCustomSound")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateEmojiCustom")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteEmojiCustom")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("public-settings-changed")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("permissions-changed")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateEmojiCustom")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteEmojiCustom")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("roles-change")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateAvatar")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("Users:NameChanged")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    // stream-notify-logged
    {
        const QJsonArray params{QJsonValue(QStringLiteral("Users:Deleted")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("deleteCustomUserStatus")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("updateCustomUserStatus")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    {
        const QJsonArray params{QJsonValue(QStringLiteral("user-status")), {true}};
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
}
