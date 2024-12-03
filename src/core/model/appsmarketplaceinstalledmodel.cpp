/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinstalledmodel.h"
#include <KLocalizedString>

AppsMarketPlaceInstalledModel::AppsMarketPlaceInstalledModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

AppsMarketPlaceInstalledModel::~AppsMarketPlaceInstalledModel() = default;

int AppsMarketPlaceInstalledModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mAppsMarketPlaceInstalledInfos.count();
}

QList<AppsMarketPlaceInstalledInfo> AppsMarketPlaceInstalledModel::appsCategories() const
{
    return mAppsMarketPlaceInstalledInfos;
}

void AppsMarketPlaceInstalledModel::setAppsCategories(const QList<AppsMarketPlaceInstalledInfo> &appsCategories)
{
    if (!mAppsMarketPlaceInstalledInfos.isEmpty()) {
        beginResetModel();
        mAppsMarketPlaceInstalledInfos.clear();
        endResetModel();
    }
    if (!appsCategories.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, appsCategories.count() - 1);
        mAppsMarketPlaceInstalledInfos = appsCategories;
        endInsertRows();
    }
}

QVariant AppsMarketPlaceInstalledModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mAppsMarketPlaceInstalledInfos.count()) {
        return {};
    }

    const AppsMarketPlaceInstalledInfo &appsMarketPlaceInfo = mAppsMarketPlaceInstalledInfos.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case AppsMarketPlaceInstalledModel::AppName:
        return appsMarketPlaceInfo.appName();
    case AppsMarketPlaceInstalledModel::AppVersion:
        return appsMarketPlaceInfo.version();
    case AppsMarketPlaceInstalledModel::Description:
        return appsMarketPlaceInfo.description();
    case AppsMarketPlaceInstalledModel::IsPrivate:
        return appsMarketPlaceInfo.isPrivate();
    case AppsMarketPlaceInstalledModel::HomePage:
        return appsMarketPlaceInfo.homePage();
    case AppsMarketPlaceInstalledModel::IsMigrated:
        return appsMarketPlaceInfo.migrated();
        // Add status !!!
    }
    return {};
}

bool AppsMarketPlaceInstalledModel::wasFilled() const
{
    return !mAppsMarketPlaceInstalledInfos.isEmpty();
}

void AppsMarketPlaceInstalledModel::clear()
{
    mAppsMarketPlaceInstalledInfos.clear();
}

#include "moc_appsmarketplaceinstalledmodel.cpp"
