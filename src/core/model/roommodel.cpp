/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
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
    connect(account, &RocketChatAccount::ownUserUiPreferencesChanged, this, [this] {
        Q_EMIT dataChanged(index(0), index(rowCount() - 1), {RoomRoles::RoomSection});
    });
}

RoomModel::~RoomModel()
{
    // VERIFY qDeleteAll(mRoomsList);
}

void RoomModel::clear()
{
    if (!mRoomsList.isEmpty()) {
        beginResetModel();
        mRoomsList.clear();
        qDeleteAll(mRoomsList);
        endResetModel();
    }
}

QVector<Room *> RoomModel::findRoomNameConstains(const QString &str) const
{
    QVector<Room *> rooms;
    for (Room *r : std::as_const(mRoomsList)) {
        if (r->displayRoomName().contains(str, Qt::CaseInsensitive)) {
            rooms.append(r);
        }
    }
    return rooms;
}

Room *RoomModel::findRoom(const QString &roomID) const
{
    for (Room *r : std::as_const(mRoomsList)) {
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
}

int RoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRoomsList.size();
}

QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoomsList.count()) {
        return {};
    }

    Room *r = mRoomsList.at(index.row());

    if (role == Qt::DisplayRole) {
        if (!r->parentRid().isEmpty()) {
            return r->fName();
        } else {
            if (mRocketChatAccount) {
                return mRocketChatAccount->useRealName() ? r->displayFName() : r->name();
            } else {
                return r->name();
            }
        }
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
        return QVariant::fromValue(r->channelType());
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
        return QVariant::fromValue(section(r));
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
    case RoomModel::RoomTeamId:
        return r->teamInfo().teamId();
    case RoomModel::RoomTeamIsMain:
        return r->teamInfo().mainTeam();
    case RoomModel::RoomLastMessageAt:
        return r->lastMessageAt();
    case RoomModel::UserOffline:
        return userOffline(r);
    case RoomModel::HideBadgeForMention:
        return r->hideBadgeForMention();
    case RoomModel::RoomGroupMentions:
        return r->groupMentions();
    case Qt::ToolTipRole:
        return generateToolTip(r);
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
    if (!addRoom(r)) {
        qCWarning(RUQOLA_ROOMS_LOG) << "Failed to add room";
    }
}

Room *RoomModel::createNewRoom()
{
    auto r = new Room(mRocketChatAccount, this);
    connect(r, &Room::alertChanged, this, &RoomModel::needToUpdateNotification);
    connect(r, &Room::unreadChanged, this, &RoomModel::needToUpdateNotification);
    connect(r, &Room::openChanged, this, &RoomModel::needToUpdateNotification);
    connect(r, &Room::needAttention, this, &RoomModel::roomNeedAttention);
    return r;
}

void RoomModel::getUnreadAlertFromAccount(bool &hasAlert, int &nbUnread) const
{
    for (int i = 0, total = mRoomsList.count(); i < total; ++i) {
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
    // Use "_id"
    QString rId = roomData.value(QLatin1String("rid")).toString();
    if (rId.isEmpty()) {
        rId = roomData.value(QLatin1String("_id")).toString();
    }
    if (!rId.isEmpty()) {
        const int roomCount = mRoomsList.size();
        for (int i = 0; i < roomCount; ++i) {
            Room *room = mRoomsList.at(i);
            if (room->roomId() == rId) {
                qCDebug(RUQOLA_ROOMS_LOG) << " void RoomModel::updateSubscriptionRoom(const QJsonArray &array) room found";
                room->updateSubscriptionRoom(roomData);
                Q_EMIT dataChanged(createIndex(i, 0), createIndex(i, 0));

                break;
            }
        }
    } else {
        qCWarning(RUQOLA_ROOMS_LOG) << "RoomModel::updateSubscriptionRoom incorrect jsonobject " << roomData;
    }
}

QString RoomModel::insertRoom(const QJsonObject &room)
{
    Room *r = createNewRoom();
    r->parseInsertRoom(room);
    qCDebug(RUQOLA_ROOMS_LOG) << "Inserting room" << r->name() << r->roomId() << r->topic();
    if (addRoom(r)) {
        return r->roomId();
    }
    return {};
}

void RoomModel::addRoom(const QJsonObject &room)
{
    Room *r = createNewRoom();
    r->parseSubscriptionRoom(room);
    qCDebug(RUQOLA_ROOMS_LOG) << "Adding room subscription" << r->name() << r->roomId() << r->topic();
    if (!addRoom(r)) {
        qCDebug(RUQOLA_ROOMS_LOG) << "Impossible to add room: " << r->name();
    }
}

Room::TeamRoomInfo RoomModel::roomFromTeamId(const QString &teamId)
{
    for (int row = 0; row < rowCount(); ++row) {
        const QModelIndex modelIndex = index(row, 0);
        if (modelIndex.data(RoomModel::RoomTeamIsMain).toBool()) {
            if (modelIndex.data(RoomModel::RoomTeamId).toString() == teamId) {
                Room::TeamRoomInfo teamInfo;
                teamInfo.teamName = modelIndex.data(RoomModel::RoomName).toString();
                teamInfo.teamIdentifier = modelIndex.data(RoomModel::RoomId).toString();
                return teamInfo;
            }
        }
    }
    return {};
}

bool RoomModel::addRoom(Room *room)
{
    qCDebug(RUQOLA_ROOMS_LOG) << " void RoomModel::addRoom(const Room &room)" << room->name();
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == room->roomId()) {
            qCDebug(RUQOLA_ROOMS_LOG) << " room already exist " << room->roomId() << " A bug ? ";
            delete room;
            return false;
        }
    }
    beginInsertRows(QModelIndex(), roomCount, roomCount);
    qCDebug(RUQOLA_ROOMS_LOG) << "Inserting room at position" << roomCount << " room name " << room->name();
    mRoomsList.append(room);
    endInsertRows();
    return true;
}

void RoomModel::removeRoom(const QString &roomId)
{
    const int roomCount = mRoomsList.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mRoomsList.at(i)->roomId() == roomId) {
            Q_EMIT roomRemoved(roomId);
            beginRemoveRows(QModelIndex(), i, i);
            mRoomsList.takeAt(i)->deleteLater();
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
            Room *room = mRoomsList.at(i);
            if (room->roomId() == rId) {
                qCDebug(RUQOLA_ROOMS_LOG) << " void RoomModel::updateRoom(const QJsonArray &array) room found";
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

MessagesModel *RoomModel::messageModel(const QString &roomId) const
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

RoomModel::Section RoomModel::section(Room *r) const
{
    const Room::RoomType roomType = r->channelType();
    if (mRocketChatAccount && mRocketChatAccount->sortUnreadOnTop() && (r->unread() > 0 || r->alert())) {
        if (!r->hideUnreadStatus()) {
            return Section::Unread;
        }
    }
    if (r->favorite() && mRocketChatAccount && mRocketChatAccount->sortFavoriteChannels()) {
        return Section::Favorites;
    } else if (r->teamInfo().mainTeam()) {
        return Section::Teams;
    }
    switch (roomType) {
    case Room::RoomType::Private: {
        if (r->parentRid().isEmpty()) {
            return Section::Rooms;
        } else {
            return Section::Discussions;
        }
    }
    case Room::RoomType::Channel: {
        if (r->parentRid().isEmpty()) {
            return Section::Rooms;
        } else {
            return Section::Discussions;
        }
    }
    case Room::RoomType::Direct: {
        return Section::PrivateMessages;
    }
    case Room::RoomType::Unknown:
        break;
    }
    return Section::Unknown;
}

bool RoomModel::userOffline(Room *r) const
{
    if (r->channelType() == Room::RoomType::Direct) {
        return mRocketChatAccount ? mRocketChatAccount->userIsOffline(r->name()) : false;
    }
    return false;
}

QString RoomModel::generateToolTip(Room *r) const
{
    if (r->teamInfo().mainTeam()) {
        return i18n("Teams Room");
    }

    switch (r->channelType()) {
    case Room::RoomType::Private:
        if (r->parentRid().isEmpty()) {
            return i18n("Private Room");
        } else {
            return i18n("Discussion Room");
        }
        break;
    case Room::RoomType::Channel:
        return i18n("Channel Room");
    case Room::RoomType::Direct: {
        return mRocketChatAccount ? mRocketChatAccount->userStatusStr(r->name()) : QString();
    }
    case Room::RoomType::Unknown:
        break;
    }
    return {};
}

QIcon RoomModel::icon(Room *r) const
{
    if (r->teamInfo().mainTeam()) {
        return QIcon::fromTheme(QStringLiteral("group"));
    }

    // TODO add team icon support.
    switch (r->channelType()) {
    case Room::RoomType::Private:
        if (r->parentRid().isEmpty()) {
            return QIcon::fromTheme(QStringLiteral("lock"));
        } else {
            // TODO use a specific icon for discussion
        }
        break;
    case Room::RoomType::Channel:
        if (r->unread() > 0 || r->alert()) {
            return QIcon::fromTheme(QStringLiteral("irc-channel-active"));
        } else {
            return QIcon::fromTheme(QStringLiteral("irc-channel-inactive"));
        }
    case Room::RoomType::Direct: {
        const QString userStatusIconFileName = mRocketChatAccount ? mRocketChatAccount->userStatusIconFileName(r->name()) : QString();
        if (userStatusIconFileName.isEmpty()) {
            return QIcon::fromTheme(QStringLiteral("user-available"));
        } else {
            return QIcon::fromTheme(userStatusIconFileName);
        }
    }
    case Room::RoomType::Unknown:
        break;
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

QString RoomModel::sectionName(Section sectionId)
{
    switch (sectionId) {
    case RoomModel::Section::Unread:
        return i18n("Unread");
    case RoomModel::Section::Favorites:
        return i18n("Favorites");
    case RoomModel::Section::Teams:
        return i18n("Teams");
    case RoomModel::Section::Rooms:
        return i18n("Rooms");
    case RoomModel::Section::Discussions:
        return i18n("Discussions");
    case RoomModel::Section::PrivateMessages:
        return i18n("Private Messages");
    case RoomModel::Section::Unknown:
        return i18n("Unknown");
    case RoomModel::Section::NSections:
        break;
    }
    return QStringLiteral("ERROR");
}

#include "moc_roommodel.cpp"
