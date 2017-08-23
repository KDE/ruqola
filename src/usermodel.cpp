/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "usermodel.h"

UsersModel::UsersModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

UsersModel::~UsersModel()
{

}

int UsersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mUsers.size();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    User user = mUsers.at(index.row());

    switch (role) {
    case UserName:
        return user.name();
    case UserId:
        return user.userId();
    case UserStatus:
        return user.status();
    case UserListRooms:
        return user.listRooms();
    }
    return {};
}

void UsersModel::removeUser()
{

}

void UsersModel::addUser()
{

}
