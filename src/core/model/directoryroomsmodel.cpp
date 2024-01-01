/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryroomsmodel.h"
#include <KLocalizedString>

DirectoryRoomsModel::DirectoryRoomsModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

DirectoryRoomsModel::~DirectoryRoomsModel() = default;

void DirectoryRoomsModel::checkFullList()
{
    setHasFullList(mRoomsInfo.count() == mRoomsInfo.total());
}

int DirectoryRoomsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mRoomsInfo.count();
}

void DirectoryRoomsModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mRoomsInfo.count();
    mRoomsInfo.parseMoreRooms(obj, RoomsInfo::Directory);
    beginInsertRows(QModelIndex(), numberOfElement, mRoomsInfo.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> DirectoryRoomsModel::excludeResizeToContentColumns() const
{
    return {Topic};
}

QList<int> DirectoryRoomsModel::hideColumns() const
{
    return {Identifier, ChannelType, CreatedDateTime, LastMessageDateTime};
}

void DirectoryRoomsModel::parseElements(const QJsonObject &roomsObj)
{
    if (rowCount() != 0) {
        beginResetModel();
        mRoomsInfo.clear();
        endResetModel();
    }
    mRoomsInfo.parseRooms(roomsObj, RoomsInfo::Directory);
    if (!mRoomsInfo.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mRoomsInfo.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QVariant DirectoryRoomsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mRoomsInfo.count()) {
        return {};
    }
    const RoomInfo &roomInfo = mRoomsInfo.at(index.row());
    const int col = index.column();
    if (role == Qt::ToolTipRole) {
        if (static_cast<DirectoryRoomsRoles>(col) == DirectoryRoomsRoles::Topic) {
            return roomInfo.topic();
        }
    }

    if (role != Qt::DisplayRole) {
        return {};
    }

    switch (static_cast<DirectoryRoomsRoles>(col)) {
    case DirectoryRoomsRoles::Name:
        return roomInfo.roomName();
    case DirectoryRoomsRoles::UsersCount:
        return roomInfo.usersCount();
    case DirectoryRoomsRoles::Topic:
        return roomInfo.topic();
    case DirectoryRoomsRoles::Identifier:
        return roomInfo.identifier();
    case DirectoryRoomsRoles::ReadOnly:
        return roomInfo.readOnly();
    case DirectoryRoomsRoles::DefaultRoom:
        return roomInfo.defaultRoom();
    case DirectoryRoomsRoles::ChannelType:
        return roomInfo.channelType();
    case DirectoryRoomsRoles::ChannelTypeStr:
        return roomInfo.channelTypeStr();
    case DirectoryRoomsRoles::LastMessageStr:
        return roomInfo.lastMessageDisplayDateTimeStr();
    case DirectoryRoomsRoles::CreatedStr:
        return roomInfo.createdRoomDisplayDateTimeStr();
    case DirectoryRoomsRoles::CreatedDateTime:
        return roomInfo.createdRoom();
    case DirectoryRoomsRoles::BelongsTo:
        return roomInfo.belongsTo();
    case DirectoryRoomsRoles::LastMessageDateTime:
        return roomInfo.lastMessage();
    }
    return {};
}

QVariant DirectoryRoomsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<DirectoryRoomsRoles>(section)) {
        case DirectoryRoomsModel::Name:
            return i18n("Name");
        case DirectoryRoomsModel::UsersCount:
            return i18n("Number Of Users");
        case DirectoryRoomsModel::Topic:
            return i18n("Topic");
        case DirectoryRoomsModel::Identifier:
            return i18n("Identifier");
        case DirectoryRoomsModel::ReadOnly:
            return i18n("Read-only");
        case DirectoryRoomsModel::DefaultRoom:
            return i18n("Default Room");
        case DirectoryRoomsModel::LastMessageStr:
            return i18n("Last Message");
        case DirectoryRoomsModel::CreatedStr:
            return i18n("Created");
        case DirectoryRoomsModel::ChannelTypeStr:
        case DirectoryRoomsModel::ChannelType:
            return i18n("Channel Type");
        case DirectoryRoomsModel::BelongsTo:
            return i18n("Belongs To");
        case DirectoryRoomsModel::CreatedDateTime:
        case DirectoryRoomsModel::LastMessageDateTime:
            return {};
        }
    }
    return {};
}

int DirectoryRoomsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(DirectoryRoomsModel::LastColumn) + 1;
    return val;
}

int DirectoryRoomsModel::total() const
{
    return mRoomsInfo.total();
}

#include "moc_directoryroomsmodel.cpp"
