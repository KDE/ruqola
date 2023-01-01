/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminpermissionsmodel.h"
#include <KLocalizedString>

AdminPermissionsModel::AdminPermissionsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AdminPermissionsModel::~AdminPermissionsModel() = default;

int AdminPermissionsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mPermissions.count();
}

QVariant AdminPermissionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminPermissionsRoles>(section)) {
        case AdminPermissionsModel::Identifier:
            return i18n("Name");
        case AdminPermissionsModel::RolesStr:
        case AdminPermissionsModel::Roles:
            return i18n("Roles");
        }
    }
    return {};
}

int AdminPermissionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(AdminPermissionsModel::LastColumn) + 1;
}

Permissions AdminPermissionsModel::permissions() const
{
    return mPermissions;
}

void AdminPermissionsModel::setPermissions(const Permissions &newPermissions)
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

void AdminPermissionsModel::setListRoleInfos(const QVector<RoleInfo> &newListRoleInfos)
{
    mListRoleInfos = newListRoleInfos;
}

QVariant AdminPermissionsModel::data(const QModelIndex &index, int role) const
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
    case AdminPermissionsModel::Identifier:
        return permissionInfo.identifier();
    case AdminPermissionsModel::Roles:
        return permissionInfo.roles();
    case AdminPermissionsModel::RolesStr:
        return permissionInfo.rolesStr().join(QLatin1Char(','));
    }
    return {};
}
