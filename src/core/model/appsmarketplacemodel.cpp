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

QList<AppsMarketPlaceInfo> AppsMarketPlaceModel::appsMarketPlaceInfos() const
{
    return mAppsMarketPlaceInfos;
}

void AppsMarketPlaceModel::setAppsMarketPlaceInfos(const QList<AppsMarketPlaceInfo> &appsMarketInfos)
{
    if (!mAppsMarketPlaceInfos.isEmpty()) {
        beginResetModel();
        mAppsMarketPlaceInfos.clear();
        endResetModel();
    }
    if (!appsMarketInfos.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, appsMarketInfos.count() - 1);
        mAppsMarketPlaceInfos = appsMarketInfos;
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
    case AppsMarketPlaceModel::AppVersion:
        return appsMarketPlaceInfo.version();
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
    case AppsMarketPlaceModel::ShortDescription:
        return appsMarketPlaceInfo.shortDescription();
    case AppsMarketPlaceModel::PurchaseType:
        return appsMarketPlaceInfo.purchaseType();
    case AppsMarketPlaceModel::DocumentationUrl:
        return appsMarketPlaceInfo.documentationUrl();
    case AppsMarketPlaceModel::Pixmap:
        return appsMarketPlaceInfo.pixmap();
    case AppsMarketPlaceModel::ModifiedDate:
        return appsMarketPlaceInfo.modifiedDate();
    case AppsMarketPlaceModel::ApplicationInformations:
        return appsMarketPlaceInfo.applicationInformations();
    case AppsMarketPlaceModel::HasPricingPlans:
        return appsMarketPlaceInfo.isPaid();
    case AppsMarketPlaceModel::RequestedApps:
        return appsMarketPlaceInfo.requested();
    }
    return {};
}

bool AppsMarketPlaceModel::wasFilled() const
{
    return !mAppsMarketPlaceInfos.isEmpty();
}

void AppsMarketPlaceModel::clear()
{
    mAppsMarketPlaceInfos.clear();
}

AppsCountInfo AppsMarketPlaceModel::appsCountInfo() const
{
    return mAppsCountInfo;
}

void AppsMarketPlaceModel::setAppsCountInfo(const AppsCountInfo &newAppsCountInfo)
{
    mAppsCountInfo = newAppsCountInfo;
}

#include "moc_appsmarketplacemodel.cpp"
