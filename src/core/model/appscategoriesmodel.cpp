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

QList<AppsCategoryInfo> AppsCategoriesModel::appsCategories() const
{
    return mAppsCategories;
}

void AppsCategoriesModel::setAppsCategories(const QList<AppsCategoryInfo> &appsCategories)
{
    if (!mAppsCategories.isEmpty()) {
        beginResetModel();
        mAppsCategories.clear();
        endResetModel();
    }
    if (!appsCategories.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, appsCategories.count() - 1);
        mAppsCategories = appsCategories;
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

    const AppsCategoryInfo &appsCategoryInfo = mAppsCategories.at(index.row());
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
