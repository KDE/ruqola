/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "teamroomsmodel.h"
#include <KLocalizedString>

TeamRoomsModel::TeamRoomsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TeamRoomsModel::~TeamRoomsModel()
{
}

int TeamRoomsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    // return mAdminRooms.count();
    return 0;
}

QVariant TeamRoomsModel::data(const QModelIndex &index, int role) const
{
#if 0
    if (index.row() < 0 || index.row() >= mAdminRooms.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const AdminRoom &adminroom = mAdminRooms.at(index.row());
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
#endif
    return {};
}

// AdminRooms TeamRoomsModel::adminRooms() const
//{
//    return mAdminRooms;
//}

// void TeamRoomsModel::setAdminRooms(const AdminRooms &adminrooms)
//{
//    if (rowCount() != 0) {
//        beginRemoveRows(QModelIndex(), 0, mAdminRooms.count() - 1);
//        mAdminRooms.clear();
//        endRemoveRows();
//    }
//    if (!adminrooms.isEmpty()) {
//        beginInsertRows(QModelIndex(), 0, adminrooms.count() - 1);
//        mAdminRooms = adminrooms;
//        endInsertRows();
//    }
//}
