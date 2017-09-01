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

#include "roommodel.h"
#include "ruqola_debug.h"
#include "rocketchataccount.h"
#include "roomwrapper.h"
#include <KLocalizedString>

#include <QAbstractItemModel>
#include <QtCore>

RoomModel::RoomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
{
}

RoomModel::~RoomModel()
{
    QDir cacheDir(mRocketChatAccount->settings()->cacheBasePath());
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }

    QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));

    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        for (const Room &m : qAsConst(mRoomsList)) {
            qCDebug(RUQOLA_LOG) << " save cache for room " << m.name();
            const QByteArray ms = RoomModel::serialize(m);
            out.writeBytes(ms, ms.size());
        }
    }
}

void RoomModel::clear()
{
    if (!mRoomsList.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        mRoomsList.clear();
        endRemoveRows();
    }
}

RoomWrapper *RoomModel::findRoom(const QString &roomID) const
{
    foreach (const Room &r, mRoomsList) {
        if (r.id() == roomID) {
            return new RoomWrapper(r);
        }
    }
    Room r;
    return new RoomWrapper(r);
}

// Clear data and refill it with data in the cache, if there is
void RoomModel::reset()
{
    if (mRocketChatAccount->settings()->cacheBasePath().isEmpty()) {
        return;
    }

    clear();
    //Laurent disable cache for the moment
/*
    QDir cacheDir(Ruqola::self()->cacheBasePath());
    // load cache
    if (cacheDir.exists(cacheDir.path())) {
        QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char *byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                QByteArray arr = QByteArray::fromRawData(byteArray, length);
                Room m = RoomModel::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                qDebug() <<" Load from cache room name: " << m.name;
                addRoom(m.id, m.name, m.selected);
            }
        }
        qCDebug(RUQOLA_LOG) << "Cache Loaded";
    }
    */
}

QHash<int, QByteArray> RoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoomName] = "name";
    roles[RoomID] = "room_id";
    roles[RoomSelected] = "selected";
    roles[RoomUnread] = "unread";
    roles[RoomType] = "type";
    roles[RoomUserName] = "username";
    roles[RoomUserID] = "userID";
    roles[RoomTopic] = "topic";
    roles[RoomMutedUsers] = "mutedUsers";
    roles[RoomJitsiTimeout] = "jitsiTimeout";
    roles[RoomRO] = "readOnly";
    roles[RoomAnnoucement] = "announcement";
    roles[RoomOpen] = "open";
    roles[RoomAlert] = "alert";
    roles[RoomOrder] = "roomorder";
    roles[RoomFavorite] = "favorite";
    return roles;
}

int RoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRoomsList.size();
}

QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    Room r = mRoomsList.at(index.row());

    switch (role) {
    case RoomModel::RoomName:
        return r.name();
    case RoomModel::RoomID:
        return r.id();
    case RoomModel::RoomSelected:
        return r.selected();
    case RoomModel::RoomType:
        return r.channelType();
    case RoomModel::RoomUserID:
        return r.userId();
    case RoomModel::RoomUserName:
        return r.userName();
    case RoomModel::RoomTopic:
        return r.topic();
    case RoomModel::RoomMutedUsers:
        return r.mutedUsers();
    case RoomModel::RoomJitsiTimeout:
        return r.jitsiTimeout();
    case RoomModel::RoomRO:
        return r.readOnly();
    case RoomModel::RoomAnnoucement:
        return r.announcement();
    case RoomModel::RoomUnread:
        return r.unread();
    case RoomModel::RoomOpen:
        return r.open();
    case RoomModel::RoomAlert:
        return r.alert();
    case RoomModel::RoomFavorite:
        return r.favorite();
    case RoomModel::RoomOrder:
    {
        QString str;
        if (r.favorite()) {
            str = i18n("Favorites");
        } else {
            if (r.channelType() == QLatin1String("c")) {
                str = i18n("Rooms");
            } else if (r.channelType() == QLatin1String("d")) {
                str = i18n("Private Message");
            } else {
                str = QString();
            }
        }
        qDebug() <<" str " << str << " name "<< r.name();
        return str;
    }
    }
    return QVariant(QStringLiteral("0"));
}

void RoomModel::addRoom(const QString &roomID, const QString &roomName, bool selected)
{
    if (roomID.isEmpty() || roomName.isEmpty()) {
        qDebug() << " Impossible to add a room";
        return;
    }
    qCDebug(RUQOLA_LOG) << "Adding room : roomId: " << roomID << " room Name " << roomName << " isSelected : " << selected;

    Room r;
    r.setId(roomID);
    r.setName(roomName);
    r.setSelected(selected);
    addRoom(r);
}

void RoomModel::addRoom(const Room &room)
{
    auto existingRoom = std::find(mRoomsList.begin(), mRoomsList.end(), room);
    bool present = (existingRoom != mRoomsList.end());

    auto i = std::upper_bound(mRoomsList.begin(), mRoomsList.end(),
                              room);
    int pos = i-mRoomsList.begin();
    qCDebug(RUQOLA_LOG) << pos;

    if (present) {
        qCDebug(RUQOLA_LOG) << "Room changed!";
        //Figure out a better way to update just the really changed message
        mRoomsList.replace(pos-1, room);
        Q_EMIT dataChanged(createIndex(1, 1), createIndex(pos, 1));
    } else {
        beginInsertRows(QModelIndex(), pos, pos);
        qCDebug(RUQOLA_LOG) << "Inserting room at position" <<pos;
        mRoomsList.insert(i, room);
        endInsertRows();
    }

    mRocketChatAccount->getMessageModelForRoom(room.id());
}

void RoomModel::updateSubscription(const QJsonArray &array)
{
    const QString actionName = array[0].toString();
    const QJsonObject roomData = array[1].toObject();
    if (actionName == QStringLiteral("removed")) {
        qDebug() << " REMOVE ROOM";
        qDebug() << " name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        Room room;
        room.setId(roomData.value(QStringLiteral("rid")).toString());
        room.setName(roomData.value(QStringLiteral("name")).toString());
        auto existingRoom = std::find(mRoomsList.begin(), mRoomsList.end(), room);
        bool present = (existingRoom != mRoomsList.end());
        if (present) {
            qDebug() << " We remove this room " <<  roomData.value(QStringLiteral("rid"));
            auto i = std::upper_bound(mRoomsList.begin(), mRoomsList.end(),
                                      room);
            int pos = i-mRoomsList.begin();
            beginRemoveRows(QModelIndex(), pos -1, pos - 1);
            mRoomsList.remove(pos);
            endRemoveRows();
        }
    } else if (actionName == QStringLiteral("inserted")) {
        qDebug() << " INSERT ROOM";
        qDebug() << " name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        addRoom(roomData.value(QStringLiteral("rid")).toString(), roomData.value(QStringLiteral("name")).toString(), false);
    } else if (actionName == QStringLiteral("updated")) {
        qDebug() << "UPDATE ROOM name " << roomData.value(QStringLiteral("name")).toString() << " rid " << roomData.value(QStringLiteral("rid")) << " roomData " << roomData;
        updateRoom(roomData);
    } else if (actionName == QStringLiteral("changed")) {
        qDebug() << "CHANGED ROOM name " << roomData.value(QStringLiteral("name")).toString() << " rid " << roomData.value(QStringLiteral("rid")) << " roomData " << roomData;
        qDebug() << " Not implementer changed room yet";
    } else {
        qDebug() << " Undefined type" << actionName;
    }
    //qDebug() << " V " << array;
}

void RoomModel::updateRoom(const QJsonObject &roomData)
{
    const QString roomName = roomData.value(QStringLiteral("name")).toString();
    if (!roomName.isEmpty()) {
        const int roomCount{mRoomsList.size()};
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i).name() == roomName) {
                //TODO change with rid and not roomname as it can be changed!
                qCDebug(RUQOLA_LOG) << " void RoomModel::updateRoom(const QJsonArray &array) room found";
                Room room = mRoomsList.at(i);
                room.parseUpdateRoom(roomData);
                mRoomsList.replace(i, room);
                Q_EMIT dataChanged(createIndex(1, 1), createIndex(i, 1));

                mRocketChatAccount->getMessageModelForRoom(room.id());
                break;
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << "RoomModel::updateRoom incorrect jsonobject "<< roomData;
    }
}

void RoomModel::updateRoom(const QString &name, const QString &roomID, const QString &topic, const QString &announcement)
{
    Room room;
    room.setId(roomID);
    room.setName(name);
    auto existingRoom = std::find(mRoomsList.begin(), mRoomsList.end(), room);
    bool present = (existingRoom != mRoomsList.end());

    auto i = std::upper_bound(mRoomsList.begin(), mRoomsList.end(), room);
    int pos = i-mRoomsList.begin();
    qCDebug(RUQOLA_LOG) << pos;

    if (present) {
        qCDebug(RUQOLA_LOG) << "Room changed!" << roomID;
    } else {
        qCWarning(RUQOLA_LOG) << " ROOM DOESNT EXIST " << roomID;
        return;
    }

    Room foundRoom = mRoomsList.value(pos - 1);
    foundRoom.setTopic(topic);
    foundRoom.setAnnouncement(announcement);
    mRoomsList.replace(pos - 1, foundRoom);
    Q_EMIT dataChanged(createIndex(1, 1), createIndex(pos, 1));

    mRocketChatAccount->getMessageModelForRoom(room.id());
}

Room RoomModel::fromJSon(const QJsonObject &o)
{
    Room r;

    r.setId(o[QStringLiteral("id")].toString());
    r.setChannelType(o[QStringLiteral("t")].toString());
    r.setName(o[QStringLiteral("name")].toString());
    r.setUserName(o[QStringLiteral("userName")].toString());
    r.setUserId(o[QStringLiteral("userID")].toString());
    r.setTopic(o[QStringLiteral("topic")].toString());
    r.setMutedUsers(o[QStringLiteral("mutedUsers")].toString());
    r.setJitsiTimeout(o[QStringLiteral("jitsiTimeout")].toDouble());
    r.setReadOnly(o[QStringLiteral("ro")].toBool());
    r.setUnread(o[QStringLiteral("unread")].toInt(0));
    r.setAnnouncement(o[QStringLiteral("announcement")].toString());
    r.setSelected(o[QStringLiteral("selected")].toBool());
    r.setFavorite(o[QStringLiteral("favorite")].toBool());
    r.setAlert(o[QStringLiteral("alert")].toBool());
    r.setOpen(o[QStringLiteral("open")].toBool());

    return r;
}

QByteArray RoomModel::serialize(const Room &r)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("id")] = r.id();
    o[QStringLiteral("t")] = r.channelType();
    o[QStringLiteral("name")] = r.name();
    o[QStringLiteral("userName")] = r.userName();
    o[QStringLiteral("userID")] = r.userId();
    o[QStringLiteral("topic")] = r.topic();
    o[QStringLiteral("mutedUsers")] = r.mutedUsers();
    o[QStringLiteral("jitsiTimeout")] = r.jitsiTimeout();
    o[QStringLiteral("ro")] = r.readOnly();
    o[QStringLiteral("unread")] = r.unread();
    o[QStringLiteral("announcement")] = r.announcement();
    o[QStringLiteral("selected")] = r.selected();
    o[QStringLiteral("favorite")] = r.favorite();
    o[QStringLiteral("alert")] = r.alert();
    o[QStringLiteral("open")] = r.open();

    d.setObject(o);
    return d.toBinaryData();
}
