/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionsmodel.h"
using namespace Qt::Literals::StringLiterals;

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
        case PermissionsModel::Identifier:
            return i18n("Name");
        case PermissionsModel::RolesStr:
        case PermissionsModel::Roles:
            return i18n("Roles");
        }
    }
    return {};
}

int PermissionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(PermissionsModel::LastColumn) + 1;
    return val;
}

Permissions PermissionsModel::permissions() const
{
    return mPermissions;
}

void PermissionsModel::setPermissions(const Permissions &newPermissions)
{
    if (!mPermissions.isEmpty()) {
        beginResetModel();
        mPermissions.clear();
        endResetModel();
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
    case PermissionsModel::Identifier:
        return permissionInfo.identifier();
    case PermissionsModel::Roles:
        return permissionInfo.roles();
    case PermissionsModel::RolesStr:
        return permissionInfo.rolesStr().join(u',');
    }
    return {};
}

#include "moc_permissionsmodel.cpp"
