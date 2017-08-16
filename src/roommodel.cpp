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
#include "ruqola.h"
#include "ruqola_debug.h"
#include "roomwrapper.h"

#include <QAbstractItemModel>
#include <QtCore>

RoomModel::RoomModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

RoomModel::~RoomModel()
{
    QDir cacheDir(Ruqola::self()->cacheBasePath());
    if (!cacheDir.exists(cacheDir.path())) {
        cacheDir.mkpath(cacheDir.path());
    }

    QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));

    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        for (const Room &m : qAsConst(mRoomsList)) {
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
        if (r.id == roomID) {
            return new RoomWrapper(r);
        }
    }
    Room r;
    return new RoomWrapper(r);
}

// Clear data and refill it with data in the cache, if there is
void RoomModel::reset()
{
    if (Ruqola::self()->cacheBasePath().isEmpty()) {
        return;
    }

    clear();

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
                addRoom(m.id, m.name, m.selected);
            }
        }
        qCDebug(RUQOLA_LOG) << "Cache Loaded";
    }
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
        return r.name;
    case RoomModel::RoomID:
        return r.id;
    case RoomModel::RoomSelected:
        return r.selected;
    case RoomModel::RoomType:
        return r.type;
    case RoomModel::RoomUserID:
        return r.userID;
    case RoomModel::RoomUserName:
        return r.userName;
    case RoomModel::RoomTopic:
        return r.topic;
    case RoomModel::RoomMutedUsers:
        return r.mutedUsers;
    case RoomModel::RoomJitsiTimeout:
        return r.jitsiTimeout;
    case RoomModel::RoomRO:
        return r.ro;
    case RoomModel::RoomAnnoucement:
        return r.mAnnouncement;
    case RoomModel::RoomUnread:
        return r.unread;
    case RoomModel::RoomOpen:
        return r.open;
    case RoomModel::RoomAlert:
        return r.alert;
    case RoomModel::RoomOrder:
    {
        QString str;
        if (r.favorite) {
            str = tr("Favorites");
        } else {
            if (r.type == QLatin1String("c")) {
                str = tr("Rooms");
            } else if (r.type == QLatin1String("d")) {
                str = tr("Private Message");
            } else {
                str = QString();
            }
        }
        qDebug() <<" str " << str << " name "<< r.name;
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
    r.id = roomID;
    r.name = roomName;
    r.selected = selected;
    addRoom(r);
}

void RoomModel::addRoom(const Room &room)
{
    auto existingRoom = std::find(mRoomsList.begin(), mRoomsList.end(), room);
    bool present = (existingRoom != mRoomsList.end());

    auto i = std::upper_bound(mRoomsList.begin(), mRoomsList.end(),
                              room);
    int pos = i-mRoomsList.begin();
    bool roomChanged = false;
    qCDebug(RUQOLA_LOG) << pos;

//     if (qFind(m_roomsList.begin(), m_roomsList.end(), room) != m_roomsList.end() && pos > 0) {
    if (present) {
//         qCDebug(RUQOLA_LOG) << (qFind(m_roomsList.begin(), m_roomsList.end(), room) - m_roomsList.begin());
//     if (pos != m_roomsList.size()) { // we're at the end
        qCDebug(RUQOLA_LOG) << "Room changed!";
        roomChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        beginInsertRows(QModelIndex(), pos, pos);
    }

    if (roomChanged) {
        mRoomsList.replace(pos-1, room);
    } else {
        qCDebug(RUQOLA_LOG) << "Inserting room at position" <<pos;
        mRoomsList.insert(i, room);
    }

    if (roomChanged) {
        emit dataChanged(createIndex(1, 1), createIndex(pos, 1));
    } else {
        endInsertRows();
    }

    Ruqola::self()->getMessageModelForRoom(room.id);
}

void RoomModel::updateSubscription(const QJsonArray &array)
{
    QString firstElement = array[0].toString();
    if (firstElement == QStringLiteral("removed")) {
        qDebug() << " REMOVE ROOM";
        const QJsonObject roomData = array[1].toObject();
        qDebug() << " name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        Room room;
        room.id = roomData.value(QStringLiteral("rid")).toString();
        room.name = roomData.value(QStringLiteral("name")).toString();
        auto existingRoom = std::find(mRoomsList.begin(), mRoomsList.end(), room);
        bool present = (existingRoom != mRoomsList.end());
        if (present) {
            qDebug() << " We remove this room " <<  roomData.value(QStringLiteral("rid"));
            auto i = std::upper_bound(mRoomsList.begin(), mRoomsList.end(),
                                      room);
            int pos = i-mRoomsList.begin();
            beginRemoveRows(QModelIndex(), pos -1 , pos - 1);
            mRoomsList.remove(pos);
            endRemoveRows();
        }

    } else if (firstElement == QStringLiteral("inserted")) {
        qDebug() << " INSERT ROOM";
        const QJsonObject roomData = array[1].toObject();
        qDebug() << " name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        addRoom(roomData.value(QStringLiteral("rid")).toString(), roomData.value(QStringLiteral("name")).toString(), false);
    } else if (firstElement == QStringLiteral("updated")) {
        qDebug() << " UPDATE ROOM";
    } else {
        qDebug() << " Undefined type" << firstElement;
    }
    qDebug() << " V " << array;
}

void RoomModel::updateRoom(const QJsonArray &array)
{
    //TODO
}

void RoomModel::updateRoom(const QString &name, const QString &roomID, const QString &topic, const QString &announcement)
{
    Room room;
    room.id = roomID;
    room.name = name;
    auto existingRoom = std::find(mRoomsList.begin(), mRoomsList.end(), room);
    bool present = (existingRoom != mRoomsList.end());

    auto i = std::upper_bound(mRoomsList.begin(), mRoomsList.end(),
                              room);
    int pos = i-mRoomsList.begin();
    bool roomChanged = false;
    qCDebug(RUQOLA_LOG) << pos;

//     if (qFind(m_roomsList.begin(), m_roomsList.end(), room) != m_roomsList.end() && pos > 0) {
    if (present) {
//         qCDebug(RUQOLA_LOG) << (qFind(m_roomsList.begin(), m_roomsList.end(), room) - m_roomsList.begin());
//     if (pos != m_roomsList.size()) { // we're at the end
        qCDebug(RUQOLA_LOG) << "Room changed!";
        roomChanged = true;
        //Figure out a better way to update just the really changed message
    } else {
        qCWarning(RUQOLA_LOG) << " ROOM DOESNT EXIST " << roomID;
        return;
    }

    if (roomChanged) {
        Room foundRoom = mRoomsList.value(pos - 1);
        foundRoom.topic = topic;
        foundRoom.mAnnouncement = announcement;
        mRoomsList.replace(pos - 1, foundRoom);
    }

    emit dataChanged(createIndex(1, 1), createIndex(pos, 1));

    Ruqola::self()->getMessageModelForRoom(room.id);
}

Room RoomModel::fromJSon(const QJsonObject &o)
{
    Room r;

    r.id = o[QStringLiteral("id")].toString();
    r.type = o[QStringLiteral("t")].toString();
    r.name = o[QStringLiteral("name")].toString();
    r.userName = o[QStringLiteral("userName")].toString();
    r.userID = o[QStringLiteral("userID")].toString();
    r.topic = o[QStringLiteral("topic")].toString();
    r.mutedUsers = o[QStringLiteral("mutedUsers")].toString();
    r.jitsiTimeout = o[QStringLiteral("jitsiTimeout")].toDouble();
    r.ro = o[QStringLiteral("ro")].toBool();
    r.unread = o[QStringLiteral("unread")].toInt(0);
    r.mAnnouncement = o[QStringLiteral("announcement")].toString();
    r.selected = o[QStringLiteral("selected")].toBool();
    r.favorite = o[QStringLiteral("favorite")].toBool();
    r.alert = o[QStringLiteral("alert")].toBool();
    r.open = o[QStringLiteral("open")].toBool();

    return r;
}

QByteArray RoomModel::serialize(const Room &r)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("id")] = r.id;
    o[QStringLiteral("t")] = r.type;
    o[QStringLiteral("name")] = r.name;
    o[QStringLiteral("userName")] = r.userName;
    o[QStringLiteral("userID")] = r.userID;
    o[QStringLiteral("topic")] = r.topic;
    o[QStringLiteral("mutedUsers")] = r.mutedUsers;
    o[QStringLiteral("jitsiTimeout")] = r.jitsiTimeout;
    o[QStringLiteral("ro")] = r.ro;
    o[QStringLiteral("unread")] = r.unread;
    o[QStringLiteral("announcement")] = r.mAnnouncement;
    o[QStringLiteral("selected")] = r.selected;
    o[QStringLiteral("favorite")] = r.favorite;
    o[QStringLiteral("alert")] = r.alert;
    o[QStringLiteral("open")] = r.open;

    d.setObject(o);
    return d.toBinaryData();
}
