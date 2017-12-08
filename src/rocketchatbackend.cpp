/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
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
#include "ruqola_debug.h"
#include "ddpapi/ddpclient.h"
#include "restapi/restapirequest.h"
#include "user.h"
#include "usersmodel.h"
#include "ruqolalogger.h"
#include "messagemodel.h"
#include "user.h"

#include <QJsonObject>

void process_publicsettings(const QJsonObject &obj, RocketChatAccount *account)
{
    account->parsePublicSettings(obj);

    //qCDebug(RUQOLA_LOG) << " configs"<<configs;
    if (account->ruqolaLogger()) {
        account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Public Settings:") + QJsonDocument(obj).toJson());
    }
}

void process_backlog(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QStringLiteral("result")).toObject();
    qCDebug(RUQOLA_LOG) << obj.value(QStringLiteral("messages")).toArray().size();
    account->rocketChatBackend()->processIncomingMessages(obj.value(QStringLiteral("messages")).toArray());
}

void rooms_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QStringLiteral("result")).toObject();
    RoomModel *model = account->roomModel();

    //qDebug() << " doc " << doc;

    QJsonArray removed = obj.value(QStringLiteral("remove")).toArray();
    //qDebug() << " rooms_parsing: room removed *************************************************" << removed;
    const QJsonArray updated = obj.value(QStringLiteral("update")).toArray();
    //qDebug() << " rooms_parsing: updated  *******************************************************: "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject roomJson = updated.at(i).toObject();
        const QString roomType = roomJson.value(QStringLiteral("t")).toString();
        if (account->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(roomJson);

            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Rooms:") + d.toJson());
        }
        if (roomType == QLatin1String("c") //Chat
            || roomType == QLatin1String("p") /*Private chat*/) {
            // let's be extra safe around crashes
            if (account->loginStatus() == DDPClient::LoggedIn) {
                Room r;
                r.parseRoom(roomJson);
                qCDebug(RUQOLA_LOG) << "Adding room" << r.id() << r.topic() << r.announcement();
                model->updateRoom(r.name(), r.id(), r.topic(), r.announcement(), r.readOnly());
            }
        }
    }
}

void getsubscription_parsing(const QJsonObject &root, RocketChatAccount *account)
{
    const QJsonObject obj = root.value(QStringLiteral("result")).toObject();
    RoomModel *model = account->roomModel();

    //qDebug() << " doc " << doc;

    QJsonArray removed = obj.value(QStringLiteral("remove")).toArray();
    //qDebug() << " room removed " << removed;
    const QJsonArray updated = obj.value(QStringLiteral("update")).toArray();
    //qDebug() << " updated : "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value(QStringLiteral("t")).toString();
        if (account->ruqolaLogger()) {
            QJsonDocument d;
            d.setObject(room);

            account->ruqolaLogger()->dataReceived(QByteArrayLiteral("Rooms subscriptions:") + d.toJson());
        }
        if (roomType == QLatin1String("c") //Chat
            || roomType == QLatin1String("p")     /*Private chat*/
            || roomType == QLatin1String("d")) {    //Direct chat) {
            const QString roomID = room.value(QStringLiteral("rid")).toString();
            MessageModel *roomModel = account->getMessageModelForRoom(roomID);

            // let's be extra safe around crashes
            if (account->loginStatus() == DDPClient::LoggedIn) {
                Room *r = new Room;
                r->parseSubscriptionRoom(room);
                qCDebug(RUQOLA_LOG) << "Adding room subscription" << r->name() << r->id() << r->topic();

                model->addRoom(r);
            }

            QJsonArray params;
            params.append(QJsonValue(roomID));
            account->ddp()->subscribeRoomMessage(roomID);

            // Load history
            params.append(QJsonValue(QJsonValue::Null));
            params.append(QJsonValue(50)); // Max number of messages to load;
            QJsonObject dateObject;
            dateObject[QStringLiteral("$date")] = QJsonValue(roomModel->lastTimestamp());
            params.append(dateObject);
            account->ddp()->method(QStringLiteral("loadHistory"), QJsonDocument(params), process_backlog);
        } else if (roomType == QLatin1String("l")) { //Live chat
            qDebug() << "Live Chat not implemented yet";
        }
    }
    //We need to load all room after get subscription to update parameters
    QJsonObject params;
    params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen
    account->ddp()->method(QStringLiteral("rooms/get"), QJsonDocument(params), rooms_parsing);
    account->ddp()->method(QStringLiteral("public-settings/get"), QJsonDocument(), process_publicsettings);
    //TODO ?
    account->ddp()->listEmojiCustom();
    account->ddp()->setDefaultStatus(User::PresenceStatus::PresenceOnline);
}

RocketChatBackend::RocketChatBackend(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
    connect(mRocketChatAccount, &RocketChatAccount::loginStatusChanged, this, &RocketChatBackend::onLoginStatusChanged);
    connect(mRocketChatAccount, &RocketChatAccount::userIDChanged, this, &RocketChatBackend::onUserIDChanged);
    connect(mRocketChatAccount, &RocketChatAccount::changed, this, &RocketChatBackend::onChanged);
    connect(mRocketChatAccount, &RocketChatAccount::added, this, &RocketChatBackend::onAdded);
}

RocketChatBackend::~RocketChatBackend()
{
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
            qCDebug(RUQOLA_LOG) <<" new message: " << o;
        }
        Message m;
        m.parseMessage(o);
        //qDebug() << " roomId"<<roomId << " add message " << m.message;
        mRocketChatAccount->getMessageModelForRoom(m.roomId())->addMessage(m);
    }
}

void RocketChatBackend::onLoginStatusChanged()
{
    if (mRocketChatAccount->loginStatus() == DDPClient::LoggedIn) {
//         qCDebug(RUQOLA_LOG) << "GETTING LIST OF ROOMS";
        QJsonObject params;
        params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen

        std::function<void(QJsonObject, RocketChatAccount *)> subscription_callback = [=](const QJsonObject &obj, RocketChatAccount *account) {
                                                                                            getsubscription_parsing(obj, account);
                                                                                        };

        mRocketChatAccount->ddp()->method(QStringLiteral("subscriptions/get"), QJsonDocument(params), subscription_callback);
        mRocketChatAccount->restApi()->setAuthToken(mRocketChatAccount->settings()->authToken());
        mRocketChatAccount->restApi()->setUserId(mRocketChatAccount->settings()->userId());
        mRocketChatAccount->restApi()->channelList();
    }
}

void RocketChatBackend::onAdded(const QJsonObject &object)
{
    QString collection = object.value(QStringLiteral("collection")).toString();

    if (collection == QLatin1String("stream-room-messages")) {
        qCDebug(RUQOLA_LOG) << "stream-room-messages : " << object;
    } else if (collection == QLatin1String("users")) {
        const QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        const QString username = fields.value(QStringLiteral("username")).toString();
        if (username == mRocketChatAccount->settings()->userName()) {
            mRocketChatAccount->settings()->setUserId(object[QStringLiteral("id")].toString());
            qCDebug(RUQOLA_LOG) << "User id set to " << mRocketChatAccount->settings()->userId();
            mRocketChatAccount->restApi()->setUserName(mRocketChatAccount->settings()->userName());
            mRocketChatAccount->restApi()->setPassword(mRocketChatAccount->settings()->password());
            mRocketChatAccount->restApi()->login();
        } else {
            //TODO add current user ? me ?
            User *user = new User;
            user->parseUser(object);
            qCDebug(RUQOLA_LOG) << " USER ADDED VALUE " << user;
            mRocketChatAccount->usersModel()->addUser(user);
        }
        qCDebug(RUQOLA_LOG) << "NEW USER ADDED: " << username << fields;
    } else if (collection == QLatin1String("rooms")) {
        qCDebug(RUQOLA_LOG) << "NEW ROOMS ADDED: " << object;
    } else if (collection == QLatin1String("stream-notify-user")) {
        qCDebug(RUQOLA_LOG) << "stream-notify-user: " << object;
    }
}

void RocketChatBackend::onChanged(const QJsonObject &object)
{
    //qDebug() << " void RocketChatBackend::onChanged(const QJsonObject &object)"<<object;
    const QString collection = object[QStringLiteral("collection")].toString();

    if (collection == QLatin1String("stream-room-messages")) {
        const QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        const QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        processIncomingMessages(contents);
    } else if (collection == QLatin1String("users")) {
        UsersModel *model = mRocketChatAccount->usersModel();
        model->updateUser(object);
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
        QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        const QString eventname = fields.value(QStringLiteral("eventName")).toString();
        const QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        qCDebug(RUQOLA_LOG) << " EVENT " << eventname << " contents " << contents << fields.value(QStringLiteral("args")).toArray().toVariantList();

        if (eventname.endsWith(QStringLiteral("/subscriptions-changed"))) {
            RoomModel *model = mRocketChatAccount->roomModel();
            model->updateSubscription(contents);
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(fields);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: subscriptions-changed:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "stream-notify-user: subscriptions-changed " << object;
            }
        } else if (eventname.endsWith(QStringLiteral("/rooms-changed"))) {
            RoomModel *model = mRocketChatAccount->roomModel();
            model->updateRoom(fields);
            if (mRocketChatAccount->ruqolaLogger()) {
                QJsonDocument d;
                d.setObject(object);
                mRocketChatAccount->ruqolaLogger()->dataReceived(QByteArrayLiteral("stream-notify-user: Room Changed:") + d.toJson());
            } else {
                qCDebug(RUQOLA_LOG) << "ROOMS CHANGED: " << object;
            }
        } else if (eventname.endsWith(QStringLiteral("/notification"))) {
            const QString message = contents.at(0).toObject()[QStringLiteral("text")].toString();
            const QString title = contents.at(0).toObject()[QStringLiteral("title")].toString();
            Q_EMIT notification(title, message);
        } else {
            qCWarning(RUQOLA_LOG) << " Unknown event ? " << eventname;
        }
    } else {
        qCDebug(RUQOLA_LOG) << " Other collection type " << collection;
    }
}

void RocketChatBackend::onUserIDChanged()
{
    const QString userId{
        mRocketChatAccount->settings()->userId()
    };
    {
        //Subscribe notification.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId).arg(QStringLiteral("notification"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe room-changed.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId).arg(QStringLiteral("rooms-changed"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe subscriptions-changed
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId).arg(QStringLiteral("subscriptions-changed"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId).arg(QStringLiteral("message"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId).arg(QStringLiteral("otr"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(userId).arg(QStringLiteral("webrtc"))));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe activeUsers
        QJsonArray params;
        params.append(QJsonValue(params));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("activeUsers"), params);
    }
    //TODO stream-notify-all ?
    {
        QJsonArray params;
        //TODO
        /*
        "params":[
              "updateAvatar",
              false
          ]
        */
        params.append(QJsonValue(params));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("stream-notify-all"), params);
    }
}
