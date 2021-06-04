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
#include "utils.h"
#include <KLocalizedString>

AdminUsersModel::AdminUsersModel(QObject *parent)
    : DirectoryUsersModel(parent)
{
}

AdminUsersModel::~AdminUsersModel()
{
}

Users::ParseType AdminUsersModel::parseType() const
{
    return Users::ParseType::Administrator;
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
        case AdminUsersModel::Status:
            return i18n("Status");
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

    const User &user = mUsers.at(index.row());
    const int col = index.column();
    switch (static_cast<AdminUsersRoles>(col)) {
    case AdminUsersRoles::Name:
        return user.name();
    case AdminUsersRoles::UserName:
        return user.userName();
    case AdminUsersRoles::Email:
        return user.userEmailsInfo().email;
    case AdminUsersRoles::Roles:
        return user.i18nRoles().join(QLatin1Char(','));
    case AdminUsersRoles::Status:
        return Utils::displaytextFromPresenceStatus(user.status());
    case AdminUsersRoles::Disabled:
        return user.active() ? i18n("Active") : i18n("Disabled");
    }
    return {};
}
