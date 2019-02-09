/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2018 Laurent Montel <montel@kde.org>
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

#include "rocketchataccount.h"
#include "rocketchatbackend.h"
#include "model/usercompletermodel.h"
#include "ruqola_debug.h"
#include "ruqola_message_debug.h"
#include "ddpapi/ddpclient.h"
#include "restapirequest.h"
#include "user.h"
#include "model/usersmodel.h"
#include "ruqolalogger.h"
#include "model/messagemodel.h"
#include "receivetypingnotificationmanager.h"
#include "file.h"
#include "user.h"

#include <QJsonObject>
#include <QJsonArray>

void process_publicsettings(const QJsonObject &obj, RocketChatAccount *account)
{
    account->parsePublicSettings(obj);

    //qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Public Settings:") + QJsonDocument(obj).toJson());
    }
}

void rooms_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    RoomModel *model = account->roomModel();

    //qDebug() << " doc " << doc;

    QJsonArray removed = obj.value(QLatin1String("remove")).toArray();
    //qDebug() << " rooms_parsing: room removed *************************************************" << removed;
    const QJsonArray updated = obj.value(QLatin1String("update")).toArray();
    //qDebug() << " rooms_parsing: updated  *******************************************************: "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject roomJson = updated.at(i).toObject();
        const QString roomType = roomJson.value(QLatin1String("t")).toString();
        if (account->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(roomJson);

            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Rooms:") + d.toJson());
        }
        if (roomType == QLatin1String("c") //Chat
            || roomType == QLatin1String("p") /*Private chat*/) {
            // let's be extra safe around crashes
            if (account->loginStatus() == DDPClient::LoggedIn) {
                model->updateRoom(roomJson);
            }
        }
    }
}

void getsubscription_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    RoomModel *model = account->roomModel();

    //qDebug() << " doc " << doc;

    const QJsonArray removed = obj.value(QLatin1String("remove")).toArray();
    qDebug() << " room removed " << removed;
    //TODO implement it.

    const QJsonArray updated = obj.value(QLatin1String("update")).toArray();
    //qDebug() << " updated : "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value(QLatin1String("t")).toString();
        if (account->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(room);

            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Rooms subscriptions:") + d.toJson());
        }
        if (roomType == QLatin1String("c") //Chat
            || roomType == QLatin1String("p")     /*Private chat*/
            || roomType == QLatin1String("d")) {    //Direct chat) {
            // let's be extra safe around crashes
            if (account->loginStatus() == DDPClient::LoggedIn) {
                Room *r = model->addRoom(room);
                account->initializeRoom(r->roomId(), roomType, r->open());
            }
        } else if (roomType == QLatin1String("l")) { //Live chat
            qCDebug(RUQOLA_LOG) << "Live Chat not implemented yet";
        }
    }
    //We need to load all room after get subscription to update parameters
    QJsonObject params;
    params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen
    account->ddp()->method(QStringLiteral("rooms/get"), QJsonDocument(params), rooms_parsing);

    //TODO ?
    account->listEmojiCustom();
    //Force set online.
    account->ddp()->setDefaultStatus(User::PresenceStatus::PresenceOnline);
}

RocketChatBackend::RocketChatBackend(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
    connect(mRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RocketChatBackend::slotLoginStatusChanged);
    connect(mRocketChatAccount, &RocketChatAccount::userIDChanged, this, &RocketChatBackend::slotUserIDChanged);
    connect(mRocketChatAccount, &RocketChatAccount::changed, this, &RocketChatBackend::slotChanged);
    connect(mRocketChatAccount, &RocketChatAccount::added, this, &RocketChatBackend::slotAdded);
    connect(mRocketChatAccount, &RocketChatAccount::removed, this, &RocketChatBackend::slotRemoved);
    connect(mRocketChatAccount, &RocketChatAccount::connectedChanged, this, &RocketChatBackend::slotConnectedChanged);
}

RocketChatBackend::~RocketChatBackend()
{
}

void RocketChatBackend::slotConnectedChanged()
{
    mRocketChatAccount->restApi()->serverInfo();
    connect(mRocketChatAccount->restApi(), &RocketChatRestApi::RestApiRequest::getServerInfoDone, this, &RocketChatBackend::parseServerVersionDone, Qt::UniqueConnection);
    mRocketChatAccount->ddp()->method(QStringLiteral("public-settings/get"), QJsonDocument(), process_publicsettings);
}

void RocketChatBackend::processIncomingMessages(const QJsonArray &messages)
{
    for (const QJsonValue &v : messages) {
        QJsonObject o = v.toObject();
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(o);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("Message:") + d.toJson());
        } else {
            qCDebug(RUQOLA_MESSAGE_LOG) <<" new message: " << o;
        }
        Message m;
        m.parseMessage(o);
        //qDebug() << " roomId"<<roomId << " add message " << m.message;
        if (MessageModel *messageModel = mRocketChatAccount->messageModelForRoom(m.roomId())) {
            messageModel->addMessage(m);
        } else {
            qCWarning(RUQOLA_MESSAGE_LOG) << " MessageModel is empty for :" << m.roomId() << " It's a bug for sure.";
        }
    }
}

void RocketChatBackend::parseOwnInfoDown(const QJsonObject &replyObject)
{
    //Move code in rocketchataccount directly ?
    //qDebug() << "replyJson " << replyJson;
    User user;
    user.setUserId(replyObject.value(QLatin1String("_id")).toString());
    user.setUserName(replyObject.value(QLatin1String("username")).toString());
    user.setStatus(replyObject.value(QLatin1String("status")).toString());
    if (user.isValid()) {
        mRocketChatAccount->usersModel()->addUser(user);
    } else {
        qCWarning(RUQOLA_LOG) << " Error during parsing user" << replyObject;
    }
}

void RocketChatBackend::slotLoginStatusChanged()
{
    if (mRocketChatAccount->loginStatus() == DDPClient::LoggedIn) {
        connect(mRocketChatAccount->restApi(), &RocketChatRestApi::RestApiRequest::getOwnInfoDone, this, &RocketChatBackend::parseOwnInfoDown, Qt::UniqueConnection);
        QJsonObject params;
        params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen

        std::function<void(QJsonObject, RocketChatAccount *)> subscription_callback = [=](const QJsonObject &obj, RocketChatAccount *account) {
                                                                                          getsubscription_parsing(obj, account);
                                                                                      };
        mRocketChatAccount->ddp()->method(QStringLiteral("subscriptions/get"), QJsonDocument(params), subscription_callback);
        mRocketChatAccount->restApi()->setAuthToken(mRocketChatAccount->settings()->authToken());
        mRocketChatAccount->restApi()->setUserId(mRocketChatAccount->settings()->userId());
        mRocketChatAccount->restApi()->getPrivateSettings();
        mRocketChatAccount->restApi()->getOwnInfo();
    }
}

void RocketChatBackend::parseServerVersionDone(const QString &version)
{
    qCDebug(RUQOLA_LOG) << " void RocketChatBackend::parseServerVersionDone(const QString &version)******************" << version;
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
    } else {
        qDebug() << " Other collection type  removed " << collection << " object "<<object;
    }
}

void RocketChatBackend::slotAdded(const QJsonObject &object)
{
    const QString collection = object.value(QLatin1String("collection")).toString();

    if (collection == QLatin1String("stream-room-messages")) {
        qCDebug(RUQOLA_LOG) << "stream-room-messages : " << object;
    } else if (collection == QLatin1String("users")) {
        const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString username = fields.value(QLatin1String("username")).toString();
        if (username == mRocketChatAccount->settings()->userName()) {
            mRocketChatAccount->settings()->setUserId(object[QStringLiteral("id")].toString());
            qCDebug(RUQOLA_LOG) << "User id set to " << mRocketChatAccount->settings()->userId();
        } else {
            //TODO add current user ? me ?
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
        qDebug() << "stream-notify-user: " << object;
    } else if (collection == QLatin1String("stream-notify-all")) {
        qCDebug(RUQOLA_LOG) << "stream-notify-user: " << object;
        qDebug() << "stream-notify-user ********************************************************" << object;
        //TODO verify that all is ok !
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
        file.parseFile(object);
        mFiles.append(file);
    } else if (collection == QLatin1String("stream-notify-room")) {
        //TODO
        qDebug() << "stream-notify-room not implemented: "<< object;
    } else {
        qDebug() << "Unknown added element: "<< object;
    }
}

void RocketChatBackend::slotChanged(const QJsonObject &object)
{
    //qDebug() << " void RocketChatBackend::onChanged(const QJsonObject &object)"<<object;
    const QString collection = object[QStringLiteral("collection")].toString();

    if (collection == QLatin1String("stream-room-messages")) {
        const QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QJsonArray contents = fields.value(QLatin1String("args")).toArray();
        processIncomingMessages(contents);
    } else if (collection == QLatin1String("users")) {
        if (mRocketChatAccount->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(object);
            mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("users: User Changed:") + d.toJson());
        } else {
            qCDebug(RUQOLA_LOG) << "USER CHANGED" << object;
        }
        mRocketChatAccount->updateUser(object);
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
        qWarning() << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();

        if (eventname.endsWith(QLatin1String("/subscriptions-changed"))) {
            RoomModel *model = mRocketChatAccount->roomModel();
            model->updateSubscription(contents);
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(fields);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: subscriptions-changed:") + d.toJson());
            } else {
                qWarning() << "stream-notify-user: subscriptions-changed " << object;
            }
        } else if (eventname.endsWith(QLatin1String("/rooms-changed"))) {
            RoomModel *model = mRocketChatAccount->roomModel();
            //qDebug() << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();
            const QJsonArray lst = fields.value(QLatin1String("args")).toArray();
            const QString actionName = lst[0].toString();
            if (actionName == QLatin1String("updated")) {
                qDebug() << " Update room " << lst;
                const QJsonObject roomData = lst[1].toObject();
                model->updateRoom(roomData);
            } else if (actionName == QLatin1String("inserted")) {
                qDebug() << "****************************************** insert new Room !!!!!" << lst;
                const QJsonObject roomData = lst[1].toObject();
                const QString rid = model->insertRoom(roomData);
                qDebug() << "rid " << rid;
                mRocketChatAccount->initializeRoom(rid, QString());
            } else if (actionName == QLatin1String("removed")) {
                qDebug() << "Remove channel" << lst;
                const QJsonObject roomData = lst[1].toObject();
                //TODO use rid
                model->removeRoom(QString());
            } else {
                qWarning() << "rooms-changed invalid actionName " << actionName;
            }
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: Room Changed:") + d.toJson());
            } else {
                qWarning() << "ROOMS CHANGED: " << object;
            }
        } else if (eventname.endsWith(QLatin1String("/notification"))) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: notification:") + d.toJson());
            } else {
                qWarning() << "NOTIFICATION: " << object;
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
            qWarning() << "stream-notify-user : OTR ? " << eventname << " contents " << contents;
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
            QString roomId = roomData.value(QLatin1String("rid")).toString();
            if (!roomId.isEmpty()) {
                MessageModel *messageModel = mRocketChatAccount->messageModelForRoom(roomId);
                Message m;
                m.parseMessage(roomData);
                //m.setMessageType(Message::System);
                //TODO add special element!See roomData QJsonObject({"_id":"u9xnnzaBQoQithsxP","msg":"You have been muted and cannot speak in this room","rid":"Dic5wZD4Zu9ze5gk3","ts":{"$date":1534166745895}})
                messageModel->addMessage(m);
            } else {
                qCWarning(RUQOLA_LOG) << "stream-notify-user : Message: ROOMID is empty ";
            }

            qWarning() << "stream-notify-user : Message  " << eventname << " contents " << contents;
        } else {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: Unknown event: ") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "Unknown change: " << object;
            }
            qWarning() << "stream-notify-user : message event " << eventname << " contents " << contents;
        }
    } else if (collection == QLatin1String("stream-notify-room")) {
        qCDebug(RUQOLA_LOG) << " stream-notify-room " << collection << " object "<<object;
        QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        const QJsonArray contents = fields.value(QLatin1String("args")).toArray();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();

        if (eventname.endsWith(QLatin1String("/deleteMessage"))) {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-room: DeleteMessage:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "Delete message" << object;
            }
            //Move code in rocketChatAccount ?

            QString roomId = eventname;
            roomId.remove(QStringLiteral("/deleteMessage"));
            MessageModel *messageModel = mRocketChatAccount->messageModelForRoom(roomId);
            if (messageModel) {
                messageModel->deleteMessage(contents.at(0).toObject()[QStringLiteral("_id")].toString());
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
            //TODO Perhaps not necessary to convert to variantlist. Need to investigate
            //qCWarning(RUQOLA_LOG) << "stream-notify-room:  typing event ? " << eventname << " content  " << contents.toVariantList();
            const QString typingUserName = contents.toVariantList().at(0).toString();
            if (typingUserName != mRocketChatAccount->settings()->userName()) {
                const bool status = contents.toVariantList().at(1).toBool();
                mRocketChatAccount->receiveTypingNotificationManager()->insertTypingNotification(roomId, typingUserName, status);
            }
        } else {
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-room:  Unknown event ?") + d.toJson());
            } else {
                qWarning() << "stream-notify-room:  Unknown event ? " << eventname;
            }
        }
    } else if (collection == QLatin1String("stream-notify-logged")) {
        QJsonObject fields = object.value(QLatin1String("fields")).toObject();
        const QString eventname = fields.value(QLatin1String("eventName")).toString();
        const QJsonArray contents = fields.value(QLatin1String("args")).toArray();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QLatin1String("args")).toArray().toVariantList();
        if (eventname == QLatin1String("roles-change")) {
            mRocketChatAccount->rolesChanged(contents);
        }
    } else {
        qWarning() << " Other collection type changed " << collection << " object "<<object;
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
    //TODO verify if we don"t send two subscription.
    const QString userId{
        mRocketChatAccount->settings()->userId()
    };
    {
        //Subscribe notification.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("notification"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe room-changed.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("rooms-changed"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe subscriptions-changed
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("subscriptions-changed"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("message"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("otr"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("webrtc"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe activeUsers
        QJsonArray params;
        params.append(QJsonValue(params));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("activeUsers"), params);
    }
    {
        //Subscribe users in room ? //TODO verify it.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId, QStringLiteral("webrtc"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-room-users"), params);
    }
    //stream-notify-all
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("updateAvatar")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("roles-change")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("updateEmojiCustom")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("deleteEmojiCustom")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("public-settings-changed")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("permissions-changed")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
    //stream-notify-logged
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("updateEmojiCustom")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    //stream-notify-logged
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("deleteEmojiCustom")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    //stream-notify-logged
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("roles-change")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    //stream-notify-logged
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("updateAvatar")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    //stream-notify-logged
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("Users:NameChanged")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
    //stream-notify-logged
    {
        const QJsonArray params{
            QJsonValue(QStringLiteral("Users:Deleted")), {
                true
            }
        };
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-logged"), params);
    }
}
