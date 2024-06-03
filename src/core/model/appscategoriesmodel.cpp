/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscategoriesmodel.h"
#include <KLocalizedString>

AppsCategoriesModel::AppsCategoriesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AppsCategoriesModel::~AppsCategoriesModel() = default;

int AppsCategoriesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mPermissions.count();
}

QVariant AppsCategoriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AdminPermissionsRoles>(section)) {
        case AppsCategoriesModel::IdentifierRole:
            return i18n("Name");
        case AppsCategoriesModel::RolesStrRole:
        case AppsCategoriesModel::RolesRole:
            return i18n("Roles");
        }
    }
    return {};
}

int AppsCategoriesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(AppsCategoriesModel::LastColumn) + 1;
    return val;
}

Permissions AppsCategoriesModel::permissions() const
{
    return mPermissions;
}

void AppsCategoriesModel::setPermissions(const Permissions &newPermissions)
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

QVariant AppsCategoriesModel::data(const QModelIndex &index, int role) const
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
    case AppsCategoriesModel::IdentifierRole:
        return permissionInfo.identifier();
    case AppsCategoriesModel::RolesRole:
        return permissionInfo.roles();
    case AppsCategoriesModel::RolesStrRole:
        return permissionInfo.rolesStr().join(QLatin1Char(','));
    }
    return {};
}

#include "moc_appscategoriesmodel.cpp"
