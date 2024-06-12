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

    const AppsMarketPlaceInfo &appsMarketPlaceInfo = mAppsMarketPlaceInfos.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case AppsMarketPlaceModel::AppName:
        return appsMarketPlaceInfo.appName();
    case AppsMarketPlaceModel::AppId:
        return appsMarketPlaceInfo.appId();
    case AppsMarketPlaceModel::Categories:
        return appsMarketPlaceInfo.categories();
    case AppsMarketPlaceModel::IsEnterpriseOnly:
        return appsMarketPlaceInfo.isEnterpriseOnly();
    case AppsMarketPlaceModel::Price:
        return appsMarketPlaceInfo.price();
    case AppsMarketPlaceModel::Description:
        return appsMarketPlaceInfo.description();
    case AppsMarketPlaceModel::PurchaseType:
        return appsMarketPlaceInfo.purchaseType();
    case AppsMarketPlaceModel::DocumentationUrl:
        return appsMarketPlaceInfo.documentationUrl();
    }
    return {};
}

#include "moc_appsmarketplacemodel.cpp"
