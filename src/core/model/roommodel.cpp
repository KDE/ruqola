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
#include "rocketchataccount.h"
#include "ruqola_rooms_debug.h"
#include "usersforroommodel.h"
#include <KLocalizedString>

#include <QDir>
#include <QFile>
#include <QIcon>
#include <QJsonArray>

RoomModel::RoomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRocketChatAccount(account)
{
}

RoomModel::~RoomModel()
{
#if 0
    if (mRocketChatAccount && mRocketChatAccount->settings()) {
        const QString cachePath = mRocketChatAccount->settings()->cacheBasePath();
        if (cachePath.isEmpty()) {
            qCWarning(RUQOLA_ROOMS_LOG) << " Cache Path is not defined";
            return;
        }
        QDir cacheDir(cachePath);
        if (!cacheDir.exists(cacheDir.path())) {
            cacheDir.mkpath(cacheDir.path());
        }

        QFile f(cacheDir.absoluteFilePath(QStringLiteral("rooms")));

        if (f.open(QIODevice::WriteOnly)) {
            QDataStream out(&f);
            for (Room *m : qAsConst(mRoomsList)) {
                qCDebug(RUQOLA_ROOMS_LOG) << " save cache for room " << m->name();
                const QByteArray ms = Room::serialize(m);
                out.writeBytes(ms.constData(), ms.size());
            }
        }
    }
#endif
    // VERIFY qDeleteAll(mRoomsList);
}

void RoomModel::clear()
{
    if (!mRoomsList.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        mRoomsList.clear();
        qDeleteAll(mRoomsList);
        endRemoveRows();
    }
}

Room *RoomModel::findRoom(const QString &roomID) const
{
    for (Room *r : qAsConst(mRoomsList)) {
        if (r->roomId() == roomID) {
            return r;
        }
    }
    return nullptr;
}

// Clear data and refill it with data in the cache, if there is
void RoomModel::reset()
{
    clear();

    if (!mRocketChatAccount) {
        return;
    }
    if (mRocketChatAccount->settings()->cacheBasePath().isEmpty()) {
        return;
    }

    // Laurent disable cache for the moment
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
                    Room m = Room::fromJSon(QJsonDocument::fromBinaryData(arr).object());
                    qDebug() <<" Load from cache room name: " << m.name;
                    addRoom(m.id, m.name, m.selected);
                }
            }
            qCDebug(RUQOLA_ROOMS_LOG) << "Cache Loaded";
        }
        */
}

int RoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRoomsList.size();
}

QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoomsList.count()) {
        return QVariant();
    }

    Room *r = mRoomsList.at(index.row());

    if (role == Qt::DisplayRole) {
        return r->displayFName();
    }

    switch (role) {
    case RoomModel::RoomName:
        return r->name();
    case RoomModel::RoomFName:
        return r->displayFName();
    case RoomModel::RoomId:
        return r->roomId();
    case RoomModel::RoomSelected:
        return r->selected();
    case RoomModel::RoomType:
        return r->channelType();
    case RoomModel::RoomOwnerUserId:
        return r->roomCreatorUserId();
    case RoomModel::RoomOwnerUserName:
        return r->roomOwnerUserName();
    case RoomModel::RoomTopic:
        return r->topic();
    case RoomModel::RoomMutedUsers:
        return r->mutedUsers();
    case RoomModel::RoomJitsiTimeout:
        return r->jitsiTimeout();
    case RoomModel::RoomReadOnly:
        return r->readOnly();
    case RoomModel::RoomAnnouncement:
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
    case RoomModel::RoomIcon:
    case Qt::DecorationRole:
        return icon(r);
    case RoomModel::RoomOtr:
        // TODO implement it.
        return {};
    case RoomModel::RoomUserMentions:
        return r->userMentions();
    case RoomModel::RoomIgnoredUsers:
        return r->ignoredUsers();
    case RoomModel::RoomAutotranslateLanguage:
        return r->autoTranslateLanguage();
    case RoomModel::RoomDirectChannelUserId:
        return r->directChannelUserId();
    case RoomModel::RoomAvatarInfo:
        return QVariant::fromValue(r->avatarInfo());
    }
    return {};
}

void RoomModel::addRoom(const QString &roomID, const QString &roomName, bool selected)
{
    if (roomID.isEmpty() || roomName.isEmpty()) {
        qCDebug(RUQOLA_ROOMS_LOG) << " Impossible to add a room";
        return;
    }
    qCDebug(RUQOLA_ROOMS_LOG) << "Adding room : roomId: " << roomID << " room Name " << roomName << " isSelected : " << selected;

    Room *r = createNewRoom();
    r->setRoomId(roomID);
    r->setName(roomName);
    r->setSelected(selected);
    addRoom(r);
}

Room *RoomModel::createNewRoom()
{
    Room *r = new Room(mRocketChatAccount, this);
    connect(r, &Room::alertChanged, this, &RoomModel::needToUpdateNotification);
    connect(r, &Room::unreadChanged, this, &RoomModel::needToUpdateNotification);
    connect(r, &Room::openChanged, this, &RoomModel::needToUpdateNotification);
    connect(r, &Room::needAttention, this, &RoomModel::roomNeedAttention);
    return r;
}

void RoomModel::getUnreadAlertFromAccount(bool &hasAlert, int &nbUnread)
{
    for (int i = 0; i < mRoomsList.count(); ++i) {
        Room *room = mRoomsList.at(i);
        if (room->open()) {
            if (room->alert()) {
                hasAlert = true;
            }
            nbUnread += room->unread();
        }
    }
}

void RoomModel::updateSubscriptionRoom(const QJsonObject &roomData)
{
    // TODO fix me!
    // Use "_id"
    QString rId = roomData.value(QLatin1String("rid")).toString();
    if (rId.isEmpty()) {
        rId = roomData.value(QLatin1String("_id")).toString();
    }
    if (!rId.isEmpty()) {
        const int roomCount = mRoomsList.size();
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i)->roomId() == rId) {
                qCDebug(RUQOLA_ROOMS_LOG) << " void RoomModel::updateSubscriptionRoom(const QJsonArray &array) room found";
                Room *room = mRoomsList.at(i);
                room->updateSubscriptionRoom(roomData);
                Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));

                break;
            }
        }
    } else {
        qCWarning(RUQOLA_ROOMS_LOG) << "RoomModel::updateRoom incorrect jsonobject " << roomData;
        // qWarning() << "RoomModel::updateSubscriptionRoom incorrect jsonobject "<< roomData;
    }
}

QString RoomModel::insertRoom(const QJsonObject &room)
{
    Room *r = createNewRoom();
    r->parseInsertRoom(room);
    qCDebug(RUQOLA_ROOMS_LOG) << "Inserting room" << r->name() << r->roomId() << r->topic();
    addRoom(r);
    return r->roomId();
}

Room *RoomModel::addRoom(const QJsonObject &room)
{
    Room *r = createNewRoom();
    r->parseSubscriptionRoom(room);
    qCDebug(RUQOLA_ROOMS_LOG) << "Adding room subscription" << r->name() << r->roomId() << r->topic();
    addRoom(r);
    return r;
}

void RoomModel::addRoom(Room *room)
{
    qCDebug(RUQOLA_ROOMS_LOG) << " void RoomModel::addRoom(const Room &room)" << room->name();
    int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == room->roomId()) {
            qCDebug(RUQOLA_ROOMS_LOG) << " room already exist " << room->roomId() << " A bug ? ";
            delete room;
            return;
        }
    }
    roomCount = mRoomsList.count();

    beginInsertRows(QModelIndex(), roomCount, roomCount);
    qCDebug(RUQOLA_ROOMS_LOG) << "Inserting room at position" << roomCount << " room name " << room->name();
    mRoomsList.append(room);
    endInsertRows();
}

void RoomModel::removeRoom(const QString &roomId)
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            beginRemoveRows(QModelIndex(), i, i);
            delete mRoomsList.takeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void RoomModel::updateSubscription(const QJsonArray &array)
{
    const QString actionName = array[0].toString();
    const QJsonObject roomData = array[1].toObject();
    if (actionName == QLatin1String("removed")) {
        qCDebug(RUQOLA_ROOMS_LOG) << "REMOVE ROOM name "
                                  << " rid " << roomData.value(QLatin1String("rid"));
        const QString id = roomData.value(QLatin1String("rid")).toString();
        removeRoom(id);
    } else if (actionName == QLatin1String("inserted")) {
        qCDebug(RUQOLA_ROOMS_LOG) << "INSERT ROOM  name " << roomData.value(QLatin1String("name")) << " rid " << roomData.value(QLatin1String("rid"));
        // TODO fix me!
        addRoom(roomData);

        // addRoom(roomData.value(QLatin1String("rid")).toString(), roomData.value(QLatin1String("name")).toString(), false);
    } else if (actionName == QLatin1String("updated")) {
        qCDebug(RUQOLA_ROOMS_LOG) << "UPDATE ROOM name " << roomData.value(QLatin1String("name")).toString() << " rid " << roomData.value(QLatin1String("rid"))
                                  << " roomData " << roomData;
        updateSubscriptionRoom(roomData);
    } else if (actionName == QLatin1String("changed")) {
        // qDebug() << "CHANGED ROOM name " << roomData.value(QLatin1String("name")).toString() << " rid " << roomData.value(QLatin1String("rid")) << " roomData
        // " << roomData;
        qCDebug(RUQOLA_ROOMS_LOG) << "CHANGED ROOM name " << roomData.value(QLatin1String("name")).toString() << " rid " << roomData.value(QLatin1String("rid"))
                                  << " roomData " << roomData;
        qCDebug(RUQOLA_ROOMS_LOG) << "RoomModel::updateSubscription Not implementer changed room yet" << array;
        updateRoom(roomData);
    } else {
        qCDebug(RUQOLA_ROOMS_LOG) << "RoomModel::updateSubscription Undefined type" << actionName;
    }
}

void RoomModel::updateRoom(const QJsonObject &roomData)
{
    qCDebug(RUQOLA_ROOMS_LOG) << " void RoomModel::updateRoom(const QJsonObject &roomData)" << roomData;
    // TODO fix me!
    // Use "_id"
    QString rId = roomData.value(QLatin1String("rid")).toString();
    if (rId.isEmpty()) {
        rId = roomData.value(QLatin1String("_id")).toString();
    }
    if (!rId.isEmpty()) {
        const int roomCount = mRoomsList.size();
        for (int i = 0; i < roomCount; ++i) {
            if (mRoomsList.at(i)->roomId() == rId) {
                qCDebug(RUQOLA_ROOMS_LOG) << " void RoomModel::updateRoom(const QJsonArray &array) room found";
                Room *room = mRoomsList.at(i);
                room->parseUpdateRoom(roomData);
                Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));

                break;
            }
        }
    } else {
        qCWarning(RUQOLA_ROOMS_LOG) << "RoomModel::updateRoom incorrect jsonobject " << roomData;
        // qWarning() << "RoomModel::updateRoom incorrect jsonobject "<< roomData;
    }
}

void RoomModel::userStatusChanged(const User &user)
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        Room *room = mRoomsList.at(i);
        if (room->name() == user.userName()) {
            const QModelIndex idx = createIndex(i, 0);
            Q_EMIT dataChanged(idx, idx);
        }
        room->usersModelForRoom()->setUserStatusChanged(user);
    }
}

UsersForRoomModel *RoomModel::usersModelForRoom(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        Room *room = mRoomsList.at(i);
        if (room->roomId() == roomId) {
            return room->usersModelForRoom();
        }
    }
    qCWarning(RUQOLA_ROOMS_LOG) << " Users model for room undefined !";
    return nullptr;
}

MessageModel *RoomModel::messageModel(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        Room *room = mRoomsList.at(i);
        if (room->roomId() == roomId) {
            return room->messageModel();
        }
    }
    return {};
}

QString RoomModel::inputMessage(const QString &roomId) const
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        Room *room = mRoomsList.at(i);
        if (room->roomId() == roomId) {
            return room->inputMessage();
        }
    }
    return {};
}

void RoomModel::setInputMessage(const QString &roomId, const QString &inputMessage)
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        Room *room = mRoomsList.at(i);
        if (room->roomId() == roomId) {
            room->setInputMessage(inputMessage);
            return;
        }
    }
}

QString RoomModel::sectionName(Room *r) const
{
    QString str;
    if (r->favorite()) {
        str = i18n("Favorites");
    } else {
        const QString channelTypeStr = r->channelType();
        if (mRocketChatAccount && mRocketChatAccount->sortUnreadOnTop() && (r->unread() > 0 || r->alert())) {
            if (channelTypeStr == QLatin1Char('p')) {
                if (r->parentRid().isEmpty()) {
                    str = i18n("Unread Rooms");
                } else {
                    str = i18n("Unread Discussions");
                }
            } else if (channelTypeStr == QLatin1Char('c')) {
                str = i18n("Unread Rooms");
            } else if (channelTypeStr == QLatin1Char('d')) {
                str = i18n("Unread Private Messages");
            }
        } else {
            if (channelTypeStr == QLatin1Char('p')) {
                if (r->parentRid().isEmpty()) {
                    str = i18n("Rooms");
                } else {
                    str = i18n("Discussions");
                }
            } else if (channelTypeStr == QLatin1Char('c')) {
                str = i18n("Rooms");
            } else if (channelTypeStr == QLatin1Char('d')) {
                str = i18n("Private Messages");
            }
        }
    }
    return str;
}

int RoomModel::order(Room *r) const
{
    int order = 0;
    // Unread on top: push down everything that isn't unread
    if (mRocketChatAccount && mRocketChatAccount->sortUnreadOnTop() && r->unread() == 0 && !r->alert()) {
        order += 20;
    }

    // Then we have favorites channels, push down everything else
    if (!r->favorite()) {
        order += 10;
    }
    const QString channelTypeStr = r->channelType();
    if (channelTypeStr == QLatin1Char('c')) {
        order += 1;
    } else if (channelTypeStr == QLatin1Char('d')) {
        order += 2;
    } else if (channelTypeStr == QLatin1Char('p')) {
        if (r->parentRid().isEmpty()) {
            order += 1;
        } else {
            order += 4;
        }
    } else {
        qCDebug(RUQOLA_ROOMS_LOG) << r->name() << "has unhandled channel type" << channelTypeStr;
        order += 5;
    }
    return order;
}

QIcon RoomModel::icon(Room *r) const
{
    if (r->channelType() == QLatin1Char('c')) {
        if (r->unread() > 0 || r->alert()) {
            return QIcon::fromTheme(QStringLiteral("irc-channel-active"));
        } else {
            return QIcon::fromTheme(QStringLiteral("irc-channel-inactive"));
        }
    } else if (r->channelType() == QLatin1Char('d')) {
        const QString userStatusIconFileName = mRocketChatAccount ? mRocketChatAccount->userStatusIconFileName(r->name()) : QString();
        if (userStatusIconFileName.isEmpty()) {
            return QIcon::fromTheme(QStringLiteral("user-available"));
        } else {
            return QIcon::fromTheme(userStatusIconFileName);
        }
    } else if (r->channelType() == QLatin1Char('p')) {
        if (r->parentRid().isEmpty()) {
            return QIcon::fromTheme(QStringLiteral("lock"));
        } else {
            // TODO use a specific icon for discussion
        }
    }
    return {};
}

QModelIndex RoomModel::indexForRoomName(const QString &roomName) const
{
    for (int row = 0; row < rowCount(); ++row) {
        const QModelIndex modelIndex = index(row, 0);
        if (modelIndex.data(RoomModel::RoomName) == roomName) {
            return modelIndex;
        }
    }
    return {};
}
