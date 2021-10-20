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

#include "adminrolesmodel.h"
#include <KLocalizedString>

AdminRolesModel::AdminRolesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminRolesModel::~AdminRolesModel()
{
}

int AdminRolesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mPermissions.count();
}

QVariant AdminRolesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminPermissionsRoles>(section)) {
        case AdminRolesModel::Identifier:
            return i18n("Name");
        case AdminRolesModel::RolesStr:
        case AdminRolesModel::Roles:
            return i18n("Roles");
        }
    }
    return QVariant();
}

int AdminRolesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminRolesModel::LastColumn) + 1;
}

Permissions AdminRolesModel::permissions() const
{
    return mPermissions;
}

void AdminRolesModel::setPermissions(const Permissions &newPermissions)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mPermissions.count() - 1);
        mPermissions.clear();
        endRemoveRows();
    }
    if (!newPermissions.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newPermissions.count() - 1);
        mPermissions = newPermissions;
        endInsertRows();
    }
}

void AdminRolesModel::setListRoleInfos(const QVector<RoleInfo> &newListRoleInfos)
{
    mListRoleInfos = newListRoleInfos;
}

QVariant AdminRolesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mPermissions.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const Permission &permissionInfo = mPermissions.at(index.row());
    const int col = index.column();
    switch (col) {
    case AdminRolesModel::Identifier:
        return permissionInfo.identifier();
    case AdminRolesModel::Roles:
        return permissionInfo.roles();
    case AdminRolesModel::RolesStr:
        return permissionInfo.rolesStr().join(QLatin1Char(','));
    }
    return {};
}
