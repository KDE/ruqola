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
    return mAppsCategories.count();
}

QVariant AppsCategoriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<AppsCategoriesRoles>(section)) {
        case AppsCategoriesModel::Identifier:
            return i18n("Name");
        case AppsCategoriesModel::Title:
        case AppsCategoriesModel::Hidden:
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

QList<AppsCategoryInfo> AppsCategoriesModel::appsCategories() const
{
    return mAppsCategories;
}

void AppsCategoriesModel::setAppsCategories(const QList<AppsCategoryInfo> &newPermissions)
{
    if (!mAppsCategories.isEmpty()) {
        beginResetModel();
        mAppsCategories.clear();
        endResetModel();
    }
    if (!newPermissions.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newPermissions.count() - 1);
        mAppsCategories = newPermissions;
        endInsertRows();
    }
}

QVariant AppsCategoriesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAppsCategories.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const AppsCategoryInfo &permissionInfo = mAppsCategories.at(index.row());
    const int col = index.column();
#if 0
    switch (col) {
    case AppsCategoriesModel::IdentifierRole:
        return permissionInfo.identifier();
    case AppsCategoriesModel::RolesRole:
        return permissionInfo.roles();
    case AppsCategoriesModel::RolesStrRole:
        return permissionInfo.rolesStr().join(QLatin1Char(','));
    }
#endif
    return {};
}

#include "moc_appscategoriesmodel.cpp"
