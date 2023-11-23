/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinrolemodel.h"
#include <KLocalizedString>

UsersInRoleModel::UsersInRoleModel(QObject *parent)
    : CustomBaseModel(parent)
{
}

UsersInRoleModel::~UsersInRoleModel() = default;

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
    mUsers.parseMoreUsers(obj, Users::UserInRoles, {}); // Don't use RoleInfo as we don't need to show it
    beginInsertRows(QModelIndex(), numberOfElement, mUsers.count() - 1);
    endInsertRows();
    checkFullList();
}

void UsersInRoleModel::parseElements(const QJsonObject &obj)
{
    if (!mUsers.isEmpty()) {
        beginResetModel();
        mUsers.clear();
        endResetModel();
    }
    mUsers.parseUsers(obj, Users::UserInRoles, {});
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
    return {};
}

int UsersInRoleModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(UsersInRoleModel::LastColumn) + 1;
    return val;
}

int UsersInRoleModel::total() const
{
    return mUsers.total();
}

#include "moc_usersinrolemodel.cpp"
