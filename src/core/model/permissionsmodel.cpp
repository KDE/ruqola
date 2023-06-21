/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionsmodel.h"
#include <KLocalizedString>

PermissionsModel::PermissionsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

PermissionsModel::~PermissionsModel() = default;

int PermissionsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mPermissions.count();
}

QVariant PermissionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminPermissionsRoles>(section)) {
        case PermissionsModel::IdentifierRole:
            return i18n("Name");
        case PermissionsModel::RolesStrRole:
        case PermissionsModel::RolesRole:
            return i18n("Roles");
        }
    }
    return {};
}

int PermissionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(PermissionsModel::LastColumn) + 1;
}

Permissions PermissionsModel::permissions() const
{
    return mPermissions;
}

void PermissionsModel::setPermissions(const Permissions &newPermissions)
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

QVariant PermissionsModel::data(const QModelIndex &index, int role) const
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
    case PermissionsModel::IdentifierRole:
        return permissionInfo.identifier();
    case PermissionsModel::RolesRole:
        return permissionInfo.roles();
    case PermissionsModel::RolesStrRole:
        return permissionInfo.rolesStr().join(QLatin1Char(','));
    }
    return {};
}

#include "moc_permissionsmodel.cpp"
