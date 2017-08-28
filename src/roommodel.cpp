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
            qDebug() << " save cache for room " << m.mName;
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
        return r.mName;
    case RoomModel::RoomID:
        return r.id;
    case RoomModel::RoomSelected:
        return r.mSelected;
    case RoomModel::RoomType:
        return r.mChannelType;
    case RoomModel::RoomUserID:
        return r.mUserId;
    case RoomModel::RoomUserName:
        return r.mUserName;
    case RoomModel::RoomTopic:
        return r.mTopic;
    case RoomModel::RoomMutedUsers:
        return r.mMutedUsers;
    case RoomModel::RoomJitsiTimeout:
        return r.mJitsiTimeout;
    case RoomModel::RoomRO:
        return r.mReadOnly;
    case RoomModel::RoomAnnoucement:
        return r.mAnnouncement;
    case RoomModel::RoomUnread:
        return r.mUnread;
    case RoomModel::RoomOpen:
        return r.mOpen;
    case RoomModel::RoomAlert:
        return r.mAlert;
    case RoomModel::RoomOrder:
    {
        QString str;
        if (r.mFavorite) {
            str = tr("Favorites");
        } else {
            if (r.mChannelType == QLatin1String("c")) {
                str = tr("Rooms");
            } else if (r.mChannelType == QLatin1String("d")) {
                str = tr("Private Message");
            } else {
                str = QString();
            }
        }
        qDebug() <<" str " << str << " name "<< r.mName;
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
    r.mName = roomName;
    r.mSelected = selected;
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
        Q_EMIT dataChanged(createIndex(1, 1), createIndex(pos, 1));
    } else {
        endInsertRows();
    }

    Ruqola::self()->getMessageModelForRoom(room.id);
}

void RoomModel::updateSubscription(const QJsonArray &array)
{
    const QString actionName = array[0].toString();
    if (actionName == QStringLiteral("removed")) {
        qDebug() << " REMOVE ROOM";
        const QJsonObject roomData = array[1].toObject();
        qDebug() << " name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        Room room;
        room.id = roomData.value(QStringLiteral("rid")).toString();
        room.mName = roomData.value(QStringLiteral("name")).toString();
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

    } else if (actionName == QStringLiteral("inserted")) {
        qDebug() << " INSERT ROOM";
        const QJsonObject roomData = array[1].toObject();
        qDebug() << " name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        addRoom(roomData.value(QStringLiteral("rid")).toString(), roomData.value(QStringLiteral("name")).toString(), false);
    } else if (actionName == QStringLiteral("updated")) {
        qDebug() << " UPDATE ROOM";
        const QJsonObject roomData = array[1].toObject();
        qDebug() << "UPDATE ROOM name " << roomData.value(QStringLiteral("name")).toString() << " rid " << roomData.value(QStringLiteral("rid")) << " roomData " << roomData;
        //updateRoom();
    } else {
        qDebug() << " Undefined type" << actionName;
    }
    qDebug() << " V " << array;
}

void RoomModel::updateRoom(const QJsonArray &array)
{
    const QJsonObject roomData = array[1].toObject();
    const QString roomName = roomData.value(QStringLiteral("name")).toString();
    if (!roomName.isEmpty()) {
        for (int i = 0; i < mRoomsList.size(); ++i) {
            if (mRoomsList.at(i).mName == roomName) {
                qCDebug(RUQOLA_LOG) << " void RoomModel::updateRoom(const QJsonArray &array) room found";
                //TODO update it.
                break;
            }
        }
    }
    //TODO
}

void RoomModel::updateRoom(const QString &name, const QString &roomID, const QString &topic, const QString &announcement)
{
    Room room;
    room.id = roomID;
    room.mName = name;
    auto existingRoom = std::find(mRoomsList.begin(), mRoomsList.end(), room);
    bool present = (existingRoom != mRoomsList.end());

    auto i = std::upper_bound(mRoomsList.begin(), mRoomsList.end(),
                              room);
    int pos = i-mRoomsList.begin();
    qCDebug(RUQOLA_LOG) << pos;

    if (present) {
        qCDebug(RUQOLA_LOG) << "Room changed!" << roomID;
    } else {
        qCWarning(RUQOLA_LOG) << " ROOM DOESNT EXIST " << roomID;
        return;
    }

    Room foundRoom = mRoomsList.value(pos - 1);
    foundRoom.mTopic = topic;
    foundRoom.mAnnouncement = announcement;
    mRoomsList.replace(pos - 1, foundRoom);
    Q_EMIT dataChanged(createIndex(1, 1), createIndex(pos, 1));

    Ruqola::self()->getMessageModelForRoom(room.id);
}

Room RoomModel::fromJSon(const QJsonObject &o)
{
    Room r;

    r.id = o[QStringLiteral("id")].toString();
    r.mChannelType = o[QStringLiteral("t")].toString();
    r.mName = o[QStringLiteral("name")].toString();
    r.mUserName = o[QStringLiteral("userName")].toString();
    r.mUserId = o[QStringLiteral("userID")].toString();
    r.mTopic = o[QStringLiteral("topic")].toString();
    r.mMutedUsers = o[QStringLiteral("mutedUsers")].toString();
    r.mJitsiTimeout = o[QStringLiteral("jitsiTimeout")].toDouble();
    r.mReadOnly = o[QStringLiteral("ro")].toBool();
    r.mUnread = o[QStringLiteral("unread")].toInt(0);
    r.mAnnouncement = o[QStringLiteral("announcement")].toString();
    r.mSelected = o[QStringLiteral("selected")].toBool();
    r.mFavorite = o[QStringLiteral("favorite")].toBool();
    r.mAlert = o[QStringLiteral("alert")].toBool();
    r.mOpen = o[QStringLiteral("open")].toBool();

    return r;
}

QByteArray RoomModel::serialize(const Room &r)
{
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("id")] = r.id;
    o[QStringLiteral("t")] = r.mChannelType;
    o[QStringLiteral("name")] = r.mName;
    o[QStringLiteral("userName")] = r.mUserName;
    o[QStringLiteral("userID")] = r.mUserId;
    o[QStringLiteral("topic")] = r.mTopic;
    o[QStringLiteral("mutedUsers")] = r.mMutedUsers;
    o[QStringLiteral("jitsiTimeout")] = r.mJitsiTimeout;
    o[QStringLiteral("ro")] = r.mReadOnly;
    o[QStringLiteral("unread")] = r.mUnread;
    o[QStringLiteral("announcement")] = r.mAnnouncement;
    o[QStringLiteral("selected")] = r.mSelected;
    o[QStringLiteral("favorite")] = r.mFavorite;
    o[QStringLiteral("alert")] = r.mAlert;
    o[QStringLiteral("open")] = r.mOpen;

    d.setObject(o);
    return d.toBinaryData();
}
