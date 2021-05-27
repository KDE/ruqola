/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "adminroomsmodel.h"
#include <KLocalizedString>

AdminRoomsModel::AdminRoomsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminRoomsModel::~AdminRoomsModel()
{
}

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
            return i18n("Number Of Messages");
        case AdminRoomsRoles::UsersCount:
            return i18n("Number Of Users");
        case AdminRoomsRoles::Topic:
            return i18n("Topic");
        case AdminRoomsRoles::Identifier:
            return i18n("Identifier");
        case AdminRoomsRoles::ReadOnly:
            return i18n("Read Only");
        case AdminRoomsRoles::DefaultRoom:
            return i18n("Default Room");
        case AdminRoomsRoles::ChannelType:
            return i18n("Type");
        case AdminRoomsRoles::ChannelTypeStr:
            return i18n("Type");
        }
    }
    return QVariant();
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
