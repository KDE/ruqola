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

#include "admininvitemodel.h"
#include <KLocalizedString>

AdminInviteModel::AdminInviteModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminInviteModel::~AdminInviteModel()
{
}

int AdminInviteModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mAdminInvites.count();
}

QVariant AdminInviteModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminInviteRoles>(section)) {
        case AdminInviteModel::Name:
            return i18n("Name");
        case AdminInviteModel::MessagesCount:
            return i18n("Number Of Messages");
        case AdminInviteModel::UsersCount:
            return i18n("Number Of Users");
        case AdminInviteModel::Topic:
            return i18n("Topic");
        case AdminInviteModel::Identifier:
            return i18n("Identifier");
        case AdminInviteModel::ReadOnly:
            return i18n("Read Only");
        case AdminInviteModel::DefaultRoom:
            return i18n("Default Room");
        case AdminInviteModel::ChannelType:
            return i18n("Type");
        case AdminInviteModel::ChannelTypeStr:
            return i18n("Type");
        }
    }
    return QVariant();
}

int AdminInviteModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminInviteModel::LastColumn) + 1;
}

const QVector<InviteInfo> &AdminInviteModel::adminInvites() const
{
    return mAdminInvites;
}

void AdminInviteModel::setAdminInvites(const QVector<InviteInfo> &newAdminInvites)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mAdminInvites.count() - 1);
        mAdminInvites.clear();
        endRemoveRows();
    }
    if (!newAdminInvites.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newAdminInvites.count() - 1);
        mAdminInvites = newAdminInvites;
        endInsertRows();
    }
}

QVariant AdminInviteModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAdminInvites.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const InviteInfo &adminroom = mAdminInvites.at(index.row());
    const int col = index.column();
#if 0
    switch (static_cast<AdminInviteRoles>(col)) {
    case AdminInviteRoles::Name:
        return adminroom.roomName();
    case AdminInviteRoles::MessagesCount:
        return adminroom.messageCount();
    case AdminInviteRoles::UsersCount:
        return adminroom.usersCount();
    case AdminInviteRoles::Topic:
        return adminroom.topic();
    case AdminInviteRoles::Identifier:
        return adminroom.identifier();
    case AdminInviteRoles::ReadOnly:
        return adminroom.readOnly();
    case AdminInviteRoles::DefaultRoom:
        return adminroom.defaultRoom();
    case AdminInviteRoles::ChannelType:
        return adminroom.channelType();
    case AdminInviteRoles::ChannelTypeStr:
        return adminroom.channelTypeStr();
    }
#endif
    return {};
}
