/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "usersinrolemodel.h"
#include "discussions/discussions.h"
#include <KLocalizedString>

UsersInRoleModel::UsersInRoleModel(QObject *parent)
    : DirectoryBaseModel(parent)
{
}

UsersInRoleModel::~UsersInRoleModel()
{
}

void UsersInRoleModel::checkFullList()
{
    setHasFullList(mUsers.count() == mUsers.total());
}

int UsersInRoleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mUsers.count();
}

QList<int> UsersInRoleModel::hideColumns() const
{
    return {UserId, UserName};
}

void UsersInRoleModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mUsers.count();
    mUsers.parseMoreUsers(obj, Users::UserInRoles);
    beginInsertRows(QModelIndex(), numberOfElement, mUsers.count() - 1);
    endInsertRows();
    checkFullList();
}

void UsersInRoleModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mUsers.count() - 1);
        mUsers.clear();
        endRemoveRows();
    }
    mUsers.parseUsers(obj, Users::UserInRoles);
    if (!mUsers.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mUsers.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

QVariant UsersInRoleModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }
    const User &user = mUsers.at(index.row());
    const int col = index.column();
    switch (static_cast<UsersInRoleRoles>(col)) {
    case UsersInRoleRoles::Name:
        return user.name().isEmpty() ? user.userName() : user.name();
    case UsersInRoleRoles::Email:
        return user.userEmailsInfo().email;
    case UsersInRoleRoles::UserId:
        return user.userId();
    case UsersInRoleRoles::UserName:
        return user.userName();
    }
    return {};
}

QVariant UsersInRoleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<UsersInRoleRoles>(section)) {
        case UsersInRoleModel::Name:
            return i18n("Name");
        case UsersInRoleModel::Email:
            return i18n("Emails");
        case UsersInRoleModel::UserId:
        case UsersInRoleModel::UserName:
            return {};
        }
    }
    return QVariant();
}

int UsersInRoleModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(UsersInRoleModel::LastColumn) + 1;
}

int UsersInRoleModel::total() const
{
    return mUsers.total();
}
