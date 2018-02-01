/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "usersmodelforroom.h"
#include "usersmodel.h"
#include "ruqola_debug.h"

UsersModelForRoom::UsersModelForRoom(QObject *parent)
    : QAbstractListModel(parent)
{
}

UsersModelForRoom::~UsersModelForRoom()
{
}

int UsersModelForRoom::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mUsers.count();
}

QVariant UsersModelForRoom::data(const QModelIndex &index, int role) const
{
    const User user = mUsers.at(index.row());
    switch (role) {
    case UserName:
        return user.userName();
    case UserIconStatus:
        return user.iconFromStatus();
    default:
        qCWarning(RUQOLA_LOG) << "Unknown usersmodel roles: " << role;
    }
    return {};
}

QHash<int, QByteArray> UsersModelForRoom::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UserName] = "username";
    roles[UserIconStatus] = "iconstatus";
    return roles;
}
