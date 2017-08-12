/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#include "rocketchatbackend.h"
#include <QtCore>
#include <QJsonObject>
#include "ruqola_debug.h"
#include "ruqola.h"
#include "ddpclient.h"

void process_backlog(const QJsonDocument &messages)
{
    qCDebug(RUQOLA_LOG) << messages.object().value(QStringLiteral("messages")).toArray().size();
    RocketChatBackend::processIncomingMessages(messages.object().value(QStringLiteral("messages")).toArray());
}

void rooms_parsing(const QJsonDocument &doc)
{
    RoomModel *model = Ruqola::self()->roomModel();

    //qDebug() << " doc " << doc;

    QJsonArray removed = doc.object().value(QStringLiteral("remove")).toArray();
    //qDebug() << " rooms_parsing: room removed *************************************************" << removed;
    const QJsonArray updated = doc.object().value(QStringLiteral("update")).toArray();
    //qDebug() << " rooms_parsing: updated  *******************************************************: "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value(QStringLiteral("t")).toString();

        if (roomType == QLatin1String("c") //Chat
                || roomType == QLatin1String("p") /*Private chat*/) {
            // let's be extra safe around crashes
            if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
                const QString roomID = room.value(QStringLiteral("_id")).toString();
                QString name = room[QStringLiteral("name")].toString();
                QString topic = room[QStringLiteral("topic")].toString();
                QString announcement = room[QStringLiteral("announcement")].toString();
                qCDebug(RUQOLA_LOG) << "Adding room" << roomID << topic << announcement;
                model->updateRoom(name, roomID, topic, announcement);
            }
        }
    }
}

void getsubscription_parsing(const QJsonDocument &doc)
{
    RoomModel *model = Ruqola::self()->roomModel();

    //qDebug() << " doc " << doc;

    QJsonArray removed = doc.object().value(QStringLiteral("remove")).toArray();
    //qDebug() << " room removed " << removed;
    const QJsonArray updated = doc.object().value(QStringLiteral("update")).toArray();
    //qDebug() << " updated : "<< updated;

    for (int i = 0; i < updated.size(); i++) {
        QJsonObject room = updated.at(i).toObject();

        const QString roomType = room.value(QStringLiteral("t")).toString();

        if (roomType == QLatin1String("c") //Chat
                || roomType == QLatin1String("p") /*Private chat*/
                ||roomType == QLatin1String("d")) { //Direct chat) {
            QString roomID = room.value(QStringLiteral("rid")).toString();
            MessageModel *roomModel = Ruqola::self()->getMessageModelForRoom(roomID);

            // let's be extra safe around crashes
            if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
                Room r;
                r.id = roomID;
                r.name = room[QStringLiteral("name")].toString();
                r.topic = room[QStringLiteral("topic")].toString();
                r.mAnnouncement = room[QStringLiteral("announcement")].toString();
                r.type = roomType;
                QJsonValue favoriteValue = room.value(QStringLiteral("f"));
                if (!favoriteValue.isUndefined()) {
                    r.favorite = favoriteValue.toBool();
                }
                //Only private room has this settings.
                if (roomType == QLatin1String("p")) {
                    r.ro = room[QStringLiteral("ro")].toString() == QLatin1String("true");
                }
                r.unread = room[QStringLiteral("unread")].toInt();
                r.open = room[QStringLiteral("open")].toBool();
                r.alert = room[QStringLiteral("alert")].toBool();
                qCDebug(RUQOLA_LOG) << "Adding room" << r.name << r.id << r.topic;

                model->addRoom(r);
            }

            QJsonArray params;
            params.append(QJsonValue(roomID));
            Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-room-messages"), params);

            // Load history
            params.append(QJsonValue(QJsonValue::Null));
            params.append(QJsonValue(50)); // Max number of messages to load;
            QJsonObject dateObject;
            dateObject[QStringLiteral("$date")] = QJsonValue(roomModel->lastTimestamp());
            params.append(dateObject);
            Ruqola::self()->ddp()->method(QStringLiteral("loadHistory"), QJsonDocument(params), process_backlog);
        } else if (roomType == QLatin1String("l")) { //Live chat
            qDebug() << "Live Chat not implemented yet";
        }
    }
    //We need to load all room after get subscription to update parameters
    QJsonObject params;
    params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen
    Ruqola::self()->ddp()->method(QStringLiteral("rooms/get"), QJsonDocument(params), rooms_parsing);
}

void rooms_callback(const QJsonDocument &doc)
{
    rooms_parsing(doc);
}

void subscription_callback(const QJsonDocument &doc)
{
    getsubscription_parsing(doc);
}

void RocketChatBackend::processIncomingMessages(const QJsonArray &messages)
{
    for (const QJsonValue &v : messages) {
        QJsonObject o = v.toObject();

        qDebug() <<" o" << o;
        Message m;
        m.parseMessage(o);
        //qDebug() << " roomId"<<roomId << " add message " << m.message;
        Ruqola::self()->getMessageModelForRoom(m.mRoomId)->addMessage(m);
    }
}

RocketChatBackend::RocketChatBackend(QObject *parent)
    : QObject(parent)
{
    connect(Ruqola::self(), &Ruqola::loginStatusChanged, this, &RocketChatBackend::onLoginStatusChanged);
    connect(Ruqola::self(), &Ruqola::userIDChanged, this, &RocketChatBackend::onUserIDChanged);
    connect(Ruqola::self()->ddp(), &DDPClient::changed, this, &RocketChatBackend::onChanged);
    connect(Ruqola::self()->ddp(), &DDPClient::added, this, &RocketChatBackend::onAdded);
}

RocketChatBackend::~RocketChatBackend()
{
}

void RocketChatBackend::onLoginStatusChanged()
{
    if (Ruqola::self()->loginStatus() == DDPClient::LoggedIn) {
//         qCDebug(RUQOLA_LOG) << "GETTING LIST OF ROOMS";
        QJsonObject params;
        params[QStringLiteral("$date")] = QJsonValue(0); // get ALL rooms we've ever seen
        Ruqola::self()->ddp()->method(QStringLiteral("subscriptions/get"), QJsonDocument(params), subscription_callback);
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
        if (username == Ruqola::self()->userName()) {
            Ruqola::self()->setUserID(object[QStringLiteral("id")].toString());
            qCDebug(RUQOLA_LOG) << "User id set to " << Ruqola::self()->userID();
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
    const QString collection = object[QStringLiteral("collection")].toString();

    if (collection == QLatin1String("stream-room-messages")) {
        const QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        const QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        processIncomingMessages(contents);
    } else if (collection == QLatin1String("users")) {
        qCDebug(RUQOLA_LOG) << "USER CHANGED" << object;
    } else if (collection == QLatin1String("rooms")) {
        qCDebug(RUQOLA_LOG) << "ROOMS CHANGED: " << object;
    } else if (collection == QLatin1String("stream-notify-user")) {
        QJsonObject fields = object.value(QStringLiteral("fields")).toObject();
        const QString eventname = fields.value(QStringLiteral("eventName")).toString();
        const QJsonArray contents = fields.value(QStringLiteral("args")).toArray();
        qDebug() << " EVENT " << eventname << " contents " << contents;

        if (eventname.endsWith(QStringLiteral("/subscriptions-changed"))) {
            qDebug() << " subscriptions-changed " << eventname;
            RoomModel *model = Ruqola::self()->roomModel();
            model->updateSubscription(contents);
        } else if (eventname.endsWith(QStringLiteral("/rooms-changed"))) {
            qDebug() << "rooms-changed " << eventname;
            RoomModel *model = Ruqola::self()->roomModel();
            model->updateRoom(contents);
        } else if (eventname.endsWith(QStringLiteral("/notification"))){
            const QString message = contents.at(0).toObject()[QStringLiteral("text")].toString();
            const QString title = contents.at(0).toObject()[QStringLiteral("title")].toString();
            Ruqola::self()->notification()->showMessage(title, message, QSystemTrayIcon::Information, 5000);
        } else {
            qDebug() << " Unknown event ? " << eventname;
        }
        qCDebug(RUQOLA_LOG) << "New notification" << fields;
    } else {
        qCDebug(RUQOLA_LOG) << " Other collection type " << collection;
    }
}

void RocketChatBackend::onUserIDChanged()
{
    {
        //Subscribe notification.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(Ruqola::self()->userID()).arg(QStringLiteral("notification"))));
        Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe room-changed.
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(Ruqola::self()->userID()).arg(QStringLiteral("rooms-changed"))));
        Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe subscriptions-changed
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(Ruqola::self()->userID()).arg(QStringLiteral("subscriptions-changed"))));
        Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe message
        QJsonArray params;
        params.append(QJsonValue(QStringLiteral("%1/%2").arg(Ruqola::self()->userID()).arg(QStringLiteral("message"))));
        Ruqola::self()->ddp()->subscribe(QStringLiteral("stream-notify-user"), params);
    }
    {
        //Subscribe activeUsers
        QJsonArray params;
        params.append(QJsonValue(params));
        Ruqola::self()->ddp()->subscribe(QStringLiteral("activeUsers"), params);
    }
    //TODO stream-notify-all ?
}
