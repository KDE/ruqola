/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsmodel.h"
#include <KLocalizedString>

AdminRoomsModel::AdminRoomsModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

AdminRoomsModel::~AdminRoomsModel() = default;

int AdminRoomsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mAdminRooms.count();
}

QVariant AdminRoomsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminRoomsRoles>(section)) {
        case AdminRoomsRoles::Name:
            return i18n("Name");
        case AdminRoomsRoles::MessagesCount:
            return i18n("Number of Messages");
        case AdminRoomsRoles::UsersCount:
            return i18n("Number of Users");
        case AdminRoomsRoles::Topic:
            return i18n("Topic");
        case AdminRoomsRoles::Identifier:
            return i18n("Identifier");
        case AdminRoomsRoles::ReadOnly:
            return i18n("Read-only");
        case AdminRoomsRoles::DefaultRoom:
            return i18n("Default Room");
        case AdminRoomsRoles::ChannelType:
        case AdminRoomsRoles::ChannelTypeStr:
            return i18n("Type");
        case AdminRoomsRoles::IsTeam:
        case AdminRoomsRoles::Featured:
            return {};
        }
    }
    return {};
}

int AdminRoomsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminRoomsRoles::LastColumn) + 1;
}

QVariant AdminRoomsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAdminRooms.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const RoomInfo &adminroom = mAdminRooms.at(index.row());
    const int col = index.column();
    switch (static_cast<AdminRoomsRoles>(col)) {
    case AdminRoomsRoles::Name:
        return adminroom.roomName();
    case AdminRoomsRoles::MessagesCount:
        return adminroom.messageCount();
    case AdminRoomsRoles::UsersCount:
        return adminroom.usersCount();
    case AdminRoomsRoles::Topic:
        return adminroom.topic();
    case AdminRoomsRoles::Identifier:
        return adminroom.identifier();
    case AdminRoomsRoles::ReadOnly:
        return adminroom.readOnly();
    case AdminRoomsRoles::DefaultRoom:
        return adminroom.defaultRoom();
    case AdminRoomsRoles::ChannelType:
        return adminroom.channelType();
    case AdminRoomsRoles::ChannelTypeStr:
        return adminroom.channelTypeStr();
    case AdminRoomsRoles::Featured:
        return adminroom.featured();
    case AdminRoomsRoles::IsTeam:
        return adminroom.isTeam();
    }

    return {};
}

RoomsInfo AdminRoomsModel::adminRooms() const
{
    return mAdminRooms;
}

void AdminRoomsModel::setAdminRooms(const RoomsInfo &adminrooms)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAdminRooms.count() - 1);
        mAdminRooms.clear();
        endRemoveRows();
    }
    if (!adminrooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, adminrooms.count() - 1);
        mAdminRooms = adminrooms;
        endInsertRows();
    }
}

int AdminRoomsModel::total() const
{
    return mAdminRooms.count();
}

void AdminRoomsModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAdminRooms.count() - 1);
        mAdminRooms.clear();
        endRemoveRows();
    }
    mAdminRooms.parseRooms(obj, RoomsInfo::Administrator);
    if (!mAdminRooms.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mAdminRooms.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

void AdminRoomsModel::checkFullList()
{
    setHasFullList(mAdminRooms.count() == mAdminRooms.total());
}

void AdminRoomsModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mAdminRooms.count();
    mAdminRooms.parseMoreRooms(obj, RoomsInfo::Directory);
    beginInsertRows(QModelIndex(), numberOfElement, mAdminRooms.count() - 1);
    endInsertRows();
    checkFullList();
}

void AdminRoomsModel::updateElement(const QJsonObject &obj)
{
    const int roomCount = mAdminRooms.count();
    const QString identifier{obj.value(QStringLiteral("_id")).toString()};
    for (int i = 0; i < roomCount; ++i) {
        if (mAdminRooms.at(i).identifier() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mAdminRooms.takeAt(i);
            endRemoveRows();
            RoomInfo room;
            room.parseRoomInfo(obj);
            beginInsertRows(QModelIndex(), i, i);
            mAdminRooms.insertRoom(i, room);
            endInsertRows();
            break;
        }
    }
}

QList<int> AdminRoomsModel::hideColumns() const
{
    return {AdminRoomsRoles::Identifier, AdminRoomsRoles::ChannelType, AdminRoomsRoles::Featured, AdminRoomsModel::IsTeam};
}

void AdminRoomsModel::removeElement(const QString &identifier)
{
    const int roomCount = mAdminRooms.count();
    for (int i = 0; i < roomCount; ++i) {
        if (mAdminRooms.at(i).identifier() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mAdminRooms.takeAt(i);
            mAdminRooms.setTotal(mAdminRooms.count()); // Update total
            endRemoveRows();
            Q_EMIT totalChanged();
            break;
        }
    }
}

#include "moc_adminroomsmodel.cpp"
