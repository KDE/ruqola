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

#include "adminusersmodel.h"
#include <KLocalizedString>

AdminUsersModel::AdminUsersModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminUsersModel::~AdminUsersModel()
{
}

int AdminUsersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mUsers.count();
}

QVariant AdminUsersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminUsersRoles>(section)) {
        case AdminUsersModel::Name:
            return i18n("Name");
        case AdminUsersModel::UserName:
            return i18n("UserName");
        case AdminUsersModel::Email:
            return i18n("Email");
        case AdminUsersModel::Roles:
            return i18n("Role");
        case AdminUsersModel::Disabled:
            return i18n("Disabled");
        }
    }
    return QVariant();
}

int AdminUsersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminUsersModel::LastColumn) + 1;
}

QVariant AdminUsersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

#if 0
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
