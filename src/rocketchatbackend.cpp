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
#include "usermodel.h"
#include "messagemodel.h"

#include <QJsonObject>

void process_backlog(const QJsonDocument &messages, RocketChatAccount *account)
{
    qCDebug(RUQOLA_LOG) << messages.object().value(QStringLiteral("messages")).toArray().size();
    account->rocketChatBackend()->processIncomingMessages(messages.object().value(QStringLiteral("messages")).toArray());
}

void rooms_parsing(const QJsonDocument &doc, RocketChatAccount *account)
{
    RoomModel *model = account->roomModel();

    //qDebug() << " doc " << doc;

    QJsonArray removed = doc.object().value(QStringLiteral("remove")).toArray();
    //qDebug() << " rooms_parsing: room removed *************************************************" << removed;
    const QJsonArray updated = doc.object().value(QStringLiteral("update")).toArray();
    //qDebug() << " rooms_parsing: updated  *******************************************************: "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject roomJson = updated.at(i).toObject();

        const QString roomType = roomJson.value(QStringLiteral("t")).toString();

        if (roomType == QLatin1String("c") //Chat
            || roomType == QLatin1String("p") /*Private chat*/) {
            // let's be extra safe around crashes
            if (account->loginStatus() == DDPClient::LoggedIn) {
                Room r;
                r.parseRoom(roomJson);
                qCDebug(RUQOLA_LOG) << "Adding room" << r.id() << r.topic() << r.announcement();
                model->updateRoom(r.name(), r.id(), r.topic(), r.announcement());
            }
        }
    }
}

void getsubscription_parsing(const QJsonDocument &doc, RocketChatAccount *account)
{
    RoomModel *model = account->roomModel();

    //qDebug() << " doc " << doc;

    QJsonArray removed = doc.object().value(QStringLiteral("remove")).toArray();
    //qDebug() << " room removed " << removed;
    const QJsonArray updated = doc.object().value(QStringLiteral("update")).toArray();
    //qDebug() << " updated : "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value(QStringLiteral("t")).toString();

        if (roomType == QLatin1String("c") //Chat
            || roomType == QLatin1String("p")     /*Private chat*/
            || roomType == QLatin1String("d")) {    //Direct chat) {
            const QString roomID = room.value(QStringLiteral("rid")).toString();
            MessageModel *roomModel = account->getMessageModelForRoom(roomID);

            // let's be extra safe around crashes
            if (account->loginStatus() == DDPClient::LoggedIn) {
                Room r;
                r.parseSubscriptionRoom(room);
                qCDebug(RUQOLA_LOG) << "Adding room subscription" << r.name() << r.id() << r.topic();

                model->addRoom(r);
            }

            QJsonArray params;
            params.append(QJsonValue(roomID));
            account->ddp()->subscribe(QStringLiteral("stream-room-messages"), params);

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

        //qDebug() <<" o" << o;
        Message m;
        m.parseMessage(o);
        //qDebug() << " roomId"<<roomId << " add message " << m.message;
        mRocketChatAccount->getMessageModelForRoom(m.mRoomId)->addMessage(m);
    }
}

void RocketChatBackend::onLoginStatusChanged()
{
    if (mRocketChatAccount->loginStatus() == DDPClient::LoggedIn) {
//         qCDebug(RUQOLA_LOG) << "GETTING LIST OF ROOMS";
        QJsonObject params;
        params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen

        std::function<void(QJsonDocument, RocketChatAccount *)> subscription_callback = [=](const QJsonDocument &doc, RocketChatAccount *account) {
                                                                                            getsubscription_parsing(doc, account);
                                                                                        };

        mRocketChatAccount->ddp()->method(QStringLiteral("subscriptions/get"), QJsonDocument(params), subscription_callback);
        mRocketChatAccount->restapi()->setAuthToken(mRocketChatAccount->settings()->authToken());
        mRocketChatAccount->restapi()->setUserId(mRocketChatAccount->settings()->userId());
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
            mRocketChatAccount->restapi()->setUserName(mRocketChatAccount->settings()->userName());
            mRocketChatAccount->restapi()->setPassword(mRocketChatAccount->settings()->password());
            mRocketChatAccount->restapi()->login();
        } else {
            //TODO add current user ? me ?
            User *user = new User;
            user->parseUser(object);
            qCDebug(RUQOLA_LOG) << " USER ADDED VALUE " << user;
            mRocketChatAccount->userModel()->addUser(user);
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
    qDebug() << " void RocketChatBackend::onChanged(const QJsonObject &object)"<<object;
    const QString collection = object[QStringLiteral("collection")].toString();

    if (collection == QLatin1String("stream-room-messages")) {
        const QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        const QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        processIncomingMessages(contents);
    } else if (collection == QLatin1String("users")) {
        qCDebug(RUQOLA_LOG) << "USER CHANGED" << object;
        UsersModel *model = mRocketChatAccount->userModel();
        model->updateUser(object);
    } else if (collection == QLatin1String("rooms")) {
        qCDebug(RUQOLA_LOG) << "ROOMS CHANGED: " << object;
    } else if (collection == QLatin1String("stream-notify-user")) {
        QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        const QString eventname = fields.value(QStringLiteral("eventName")).toString();
        const QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        qDebug() << " EVENT " << eventname << " contents " << contents << fields.value(QStringLiteral("args")).toArray().toVariantList();

        if (eventname.endsWith(QStringLiteral("/subscriptions-changed"))) {
            qDebug() << " subscriptions-changed " << eventname;
            RoomModel *model = mRocketChatAccount->roomModel();
            model->updateSubscription(contents);
        } else if (eventname.endsWith(QStringLiteral("/rooms-changed"))) {
            qDebug() << "rooms-changed " << eventname;
            RoomModel *model = mRocketChatAccount->roomModel();
            model->updateRoom(fields);
        } else if (eventname.endsWith(QStringLiteral("/notification"))) {
            const QString message = contents.at(0).toObject()[QStringLiteral("text")].toString();
            const QString title = contents.at(0).toObject()[QStringLiteral("title")].toString();
            Q_EMIT notification(title, message);
        } else {
            qCWarning(RUQOLA_LOG) << " Unknown event ? " << eventname;
        }
        qCDebug(RUQOLA_LOG) << "New notification" << fields;
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
        //Subscribe activeUsers
        QJsonArray params;
        params.append(QJsonValue(params));
        mRocketChatAccount->ddp()->subscribe(QStringLiteral("activeUsers"), params);
    }
    //TODO stream-notify-all ?
}
