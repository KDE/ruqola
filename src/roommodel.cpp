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

RoomModel::RoomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
{
}

RoomModel::~RoomModel()
{
    if (mRocketChatAccount) {
        QDir cacheDir(mRocketChatAccount->settings()->cacheBasePath());
        if (!cacheDir.exists(cacheDir.path())) {
            cacheDir.mkpath(cacheDir.path());
        }

        QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));

        if (f.open(QIODevice::WriteOnly)) {
            QDataStream out(&f);
            for (Room *m : qAsConst(mRoomsList)) {
                qCDebug(RUQOLA_LOG) << " save cache for room " << m->name();
                const QByteArray ms = Room::serialize(m);
                out.writeBytes(ms, ms.size());
            }
        }
    }
    qDeleteAll(mRoomsList);
}

void RoomModel::clear()
{
    if (!mRoomsList.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        qDeleteAll(mRoomsList);
        mRoomsList.clear();
        endRemoveRows();
    }
}

RoomWrapper *RoomModel::findRoom(const QString &roomID) const
{
    foreach (Room *r, qAsConst(mRoomsList)) {
        if (r->id() == roomID) {
            return new RoomWrapper(r);
        }
    }
    return nullptr;
    //Leak!
    Room *r = new Room;
    return new RoomWrapper(r);
}

// Clear data and refill it with data in the cache, if there is
void RoomModel::reset()
{
    if (!mRocketChatAccount) {
        return;
    }
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
    roles[RoomRo] = "readOnly";
    roles[RoomAnnoucement] = "announcement";
    roles[RoomOpen] = "open";
    roles[RoomAlert] = "alert";
    roles[RoomOrder] = "roomorder";
    roles[RoomFavorite] = "favorite";
    roles[RoomSection] = "sectionname";
    return roles;
}

int RoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mRoomsList.size();
}

QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    Room *r = mRoomsList.at(index.row());

    switch (role) {
    case RoomModel::RoomName:
        return r->name();
    case RoomModel::RoomID:
        return r->id();
    case RoomModel::RoomSelected:
        return r->selected();
    case RoomModel::RoomType:
        return r->channelType();
    case RoomModel::RoomUserID:
        return r->userId();
    case RoomModel::RoomUserName:
        return r->userName();
    case RoomModel::RoomTopic:
        return r->topic();
    case RoomModel::RoomMutedUsers:
        return r->mutedUsers();
    case RoomModel::RoomJitsiTimeout:
        return r->jitsiTimeout();
    case RoomModel::RoomRo:
        return r->readOnly();
    case RoomModel::RoomAnnoucement:
        return r->announcement();
    case RoomModel::RoomUnread:
        return r->unread();
    case RoomModel::RoomOpen:
        return r->open();
    case RoomModel::RoomAlert:
        return r->alert();
    case RoomModel::RoomFavorite:
        return r->favorite();
    case RoomModel::RoomSection:
        return sectionName(r);
    case RoomModel::RoomOrder:
        return order(r);
    }
    return {};
}

void RoomModel::addRoom(const QString &roomID, const QString &roomName, bool selected)
{
    if (roomID.isEmpty() || roomName.isEmpty()) {
        qDebug() << " Impossible to add a room";
        return;
    }
    qCDebug(RUQOLA_LOG) << "Adding room : roomId: " << roomID << " room Name " << roomName << " isSelected : " << selected;

    Room *r = new Room;
    r->setId(roomID);
    r->setName(roomName);
    r->setSelected(selected);
    addRoom(r);
}

void RoomModel::addRoom(Room *room)
{
    qDebug() << " void RoomModel::addRoom(const Room &room)"<<room->name();
    const int roomCount{
        mRoomsList.count()
    };
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->id() == room->id()) {
            delete mRoomsList.takeAt(i);
            break;
        }
    }
    beginInsertRows(QModelIndex(), mRoomsList.count(), mRoomsList.count());
    qCDebug(RUQOLA_LOG) << "Inserting room at position" <<roomCount;
    mRoomsList.append(room);
    endInsertRows();
    if (mRocketChatAccount) {
        mRocketChatAccount->getMessageModelForRoom(room->id());
    }
}

void RoomModel::updateSubscription(const QJsonArray &array)
{
    const QString actionName = array[0].toString();
    const QJsonObject roomData = array[1].toObject();
    if (actionName == QStringLiteral("removed")) {
        qDebug() << "REMOVE ROOM name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        const QString id = roomData.value(QStringLiteral("rid")).toString();
        const int roomCount{
            mRoomsList.count()
        };
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i)->id() == id) {
                beginRemoveRows(QModelIndex(), i, i);
                mRoomsList.remove(i);
                endRemoveRows();
                break;
            }
        }
    } else if (actionName == QStringLiteral("inserted")) {
        //qDebug() << "INSERT ROOM  name " << roomData.value(QStringLiteral("name")) << " rid " << roomData.value(QStringLiteral("rid"));
        addRoom(roomData.value(QStringLiteral("rid")).toString(), roomData.value(QStringLiteral("name")).toString(), false);
    } else if (actionName == QStringLiteral("updated")) {
        //qDebug() << "UPDATE ROOM name " << roomData.value(QStringLiteral("name")).toString() << " rid " << roomData.value(QStringLiteral("rid")) << " roomData " << roomData;
        updateRoom(roomData);
    } else if (actionName == QStringLiteral("changed")) {
        //qDebug() << "CHANGED ROOM name " << roomData.value(QStringLiteral("name")).toString() << " rid " << roomData.value(QStringLiteral("rid")) << " roomData " << roomData;
        //qDebug() << " Not implementer changed room yet";
        updateRoom(roomData);
    } else {
        qDebug() << " Undefined type" << actionName;
    }
}

void RoomModel::updateRoom(const QJsonObject &roomData)
{
    const QString roomName = roomData.value(QStringLiteral("name")).toString();
    if (!roomName.isEmpty()) {
        const int roomCount{
            mRoomsList.size()
        };
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i)->name() == roomName) {
                //TODO change with rid and not roomname as it can be changed!
                qCDebug(RUQOLA_LOG) << " void RoomModel::updateRoom(const QJsonArray &array) room found";
                Room *room = mRoomsList.at(i);
                room->parseUpdateRoom(roomData);
                Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));

                if (mRocketChatAccount) {
                    mRocketChatAccount->getMessageModelForRoom(room->id());
                }
                break;
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << "RoomModel::updateRoom incorrect jsonobject "<< roomData;
    }
}

void RoomModel::updateRoom(const QString &name, const QString &roomId, const QString &topic, const QString &announcement, bool readOnly)
{
    const int roomCount{
        mRoomsList.count()
    };
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->id() == roomId) {
            qCDebug(RUQOLA_LOG) << "Room changed!" << roomId;
            Room *foundRoom = mRoomsList.value(i);
            foundRoom->setTopic(topic);
            foundRoom->setAnnouncement(announcement);
            foundRoom->setName(name);
            foundRoom->setReadOnly(readOnly);
            Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));

            if (mRocketChatAccount) {
                mRocketChatAccount->getMessageModelForRoom(roomId);
            }
            return;
        }
    }
    qCWarning(RUQOLA_LOG) << " ROOM DOESNT EXIST " << roomId;
}

QString RoomModel::sectionName(Room *r) const
{
    QString str;
    if (r->favorite()) {
        str = i18n("Favorites");
    } else {
        if (r->channelType() == QLatin1String("c")) {
            str = i18n("Rooms");
        } else if (r->channelType() == QLatin1String("d")) {
            str = i18n("Private Message");
        } else {
            str = QString();
        }
    }
    return str;
}

int RoomModel::order(Room *r) const
{
    int order = 0;
    //First item are favorites channels
    if (!r->favorite()) {
        order += 3;
    }
    const QString channelTypeStr{
        r->channelType()
    };
    if (channelTypeStr == QLatin1String("c")) {
        order += 1;
    } else if (channelTypeStr == QLatin1String("d")) {
        order += 2;
    } else {
        order += 3;
    }
    return order;
}
