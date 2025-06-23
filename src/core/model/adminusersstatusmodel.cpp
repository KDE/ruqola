/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminusersstatusmodel.h"

#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
AdminUsersStatusModel::AdminUsersStatusModel(QObject *parent)
    : AdminUsersBaseModel(parent)
{
}

AdminUsersStatusModel::~AdminUsersStatusModel() = default;

QVariant AdminUsersStatusModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminUsersAllRoles>(section)) {
        case AdminUsersStatusModel::Name:
            return i18n("Name");
        case AdminUsersStatusModel::UserName:
            return i18n("UserName");
        case AdminUsersStatusModel::Email:
            return i18n("Email");
        case AdminUsersStatusModel::Roles:
            return i18n("Role");
        case AdminUsersStatusModel::UserId:
        case AdminUsersAllRoles::Administrator:
            return {};
        }
    }
    return {};
}

int AdminUsersStatusModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(AdminUsersStatusModel::LastColumn) + 1;
    return val;
}

QList<int> AdminUsersStatusModel::hideColumns() const
{
    return {AdminUsersAllRoles::UserId, AdminUsersAllRoles::Administrator};
}

QVariant AdminUsersStatusModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mUsers.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const User &user = mUsers.at(index.row());
    const int col = index.column();
    switch (static_cast<AdminUsersAllRoles>(col)) {
    case AdminUsersAllRoles::Name: {
        const QString name = user.name();
        return name.isEmpty() ? user.userName() : name;
    }
    case AdminUsersAllRoles::UserName:
        return user.userName();
    case AdminUsersAllRoles::Email:
        return user.userEmailsInfo().email;
    case AdminUsersAllRoles::Roles:
        return user.i18nRoles().join(u',');
    case AdminUsersAllRoles::UserId:
        return user.userId();
    case AdminUsersAllRoles::Administrator:
        return user.roles().contains(u"admin"_s);
    }
    return {};
}

#include "moc_adminusersstatusmodel.cpp"
