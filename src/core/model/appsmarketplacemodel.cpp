/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplacemodel.h"
#include <KLocalizedString>

AppsMarketPlaceModel::AppsMarketPlaceModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AppsMarketPlaceModel::~AppsMarketPlaceModel() = default;

int AppsMarketPlaceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mAppsMarketPlaceInfos.count();
}

int AppsMarketPlaceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    constexpr int val = static_cast<int>(AppsMarketPlaceModel::LastColumn) + 1;
    return val;
}

QList<AppsMarketPlaceInfo> AppsMarketPlaceModel::appsCategories() const
{
    return mAppsMarketPlaceInfos;
}

void AppsMarketPlaceModel::setAppsCategories(const QList<AppsMarketPlaceInfo> &appsCategories)
{
    if (!mAppsMarketPlaceInfos.isEmpty()) {
        beginResetModel();
        mAppsMarketPlaceInfos.clear();
        endResetModel();
    }
    if (!appsCategories.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, appsCategories.count() - 1);
        mAppsMarketPlaceInfos = appsCategories;
        endInsertRows();
    }
}

QVariant AppsMarketPlaceModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAppsMarketPlaceInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const AppsMarketPlaceInfo &appsCategoryInfo = mAppsMarketPlaceInfos.at(index.row());
    const int col = index.column();
#if 0
    switch (col) {
    case AppsMarketPlaceModel::IdentifierRole:
        return permissionInfo.identifier();
    case AppsMarketPlaceModel::RolesRole:
        return permissionInfo.roles();
    case AppsMarketPlaceModel::RolesStrRole:
        return permissionInfo.rolesStr().join(QLatin1Char(','));
    }
#endif
    return {};
}

#include "moc_appsmarketplacemodel.cpp"
