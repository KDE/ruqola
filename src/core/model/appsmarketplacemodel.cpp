/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

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

void AppsMarketPlaceModel::clearInstalledAppsInformation()
{
    if (mAppsMarketPlaceInfos.isEmpty()) {
        return;
    }
    beginResetModel();
    for (AppsMarketPlaceInfo &info : mAppsMarketPlaceInfos) {
        if (info.installedInfo().isValid()) {
            info.clearInstalledInfo();
        }
    }
    for (const AppsMarketPlaceInfo &info : std::as_const(mAppsMarketPlaceInfos)) {
        if (!info.isValid()) {
            mAppsMarketPlaceInfos.removeAll(info);
        }
    }
    endResetModel();
}

void AppsMarketPlaceModel::removeApp(const QString &appId)
{
    for (AppsMarketPlaceInfo &info : mAppsMarketPlaceInfos) {
        if (info.appId() == appId.toLatin1()) {
            info.clearInstalledInfo();
            if (!info.isValid()) {
                mAppsMarketPlaceInfos.removeAll(info);
            }
            beginResetModel();
            endResetModel();
            break;
        }
    }
}

void AppsMarketPlaceModel::updateAppStatus(const QString &appId, const QString &status)
{
    for (AppsMarketPlaceInfo &info : mAppsMarketPlaceInfos) {
        if (info.appId() == appId.toLatin1()) {
            info.changeApplicationStatus(status);
            beginResetModel();
            endResetModel();
            break;
        }
    }
}

void AppsMarketPlaceModel::addInstalledInfo(const AppsMarketPlaceInstalledInfo &installedInfo)
{
    beginResetModel();
    bool found = false;
    for (AppsMarketPlaceInfo &info : mAppsMarketPlaceInfos) {
        if (info.appId() == installedInfo.appId()) {
            info.setInstalledInfo(installedInfo);
            found = true;
            break;
        }
    }
    if (!found) {
        AppsMarketPlaceInfo info;
        info.setInstalledInfo(installedInfo);
        mAppsMarketPlaceInfos.append(info);
    }
    endResetModel();
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

QByteArray AppsMarketPlaceModel::appId(const AppsMarketPlaceInfo &appsMarketPlaceInfo) const
{
    if (!appsMarketPlaceInfo.appId().isEmpty()) {
        return appsMarketPlaceInfo.appId();
    }
    const AppsMarketPlaceInstalledInfo installedInfo = appsMarketPlaceInfo.installedInfo();
    if (installedInfo.isValid()) {
        return installedInfo.appId();
    }
    return {};
}

QString AppsMarketPlaceModel::appName(const AppsMarketPlaceInfo &appsMarketPlaceInfo) const
{
    if (!appsMarketPlaceInfo.appId().isEmpty()) {
        return appsMarketPlaceInfo.appName();
    }
    const AppsMarketPlaceInstalledInfo installedInfo = appsMarketPlaceInfo.installedInfo();
    if (installedInfo.isValid()) {
        return installedInfo.appName();
    }
    return {};
}

QPixmap AppsMarketPlaceModel::pixmap(const AppsMarketPlaceInfo &appsMarketPlaceInfo) const
{
    const AppsMarketPlaceInstalledInfo installedInfo = appsMarketPlaceInfo.installedInfo();
    if (installedInfo.isValid()) {
        return installedInfo.pixmap();
    }
    if (!appsMarketPlaceInfo.pixmap().isNull()) {
        return appsMarketPlaceInfo.pixmap();
    }
    return {};
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
        return appName(appsMarketPlaceInfo);
    case AppsMarketPlaceModel::AppVersion:
        return appsMarketPlaceInfo.version();
    case AppsMarketPlaceModel::AppId:
        return appId(appsMarketPlaceInfo);
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
        return pixmap(appsMarketPlaceInfo);
    case AppsMarketPlaceModel::ModifiedDate:
        return appsMarketPlaceInfo.modifiedDate();
    case AppsMarketPlaceModel::ApplicationInformations:
        return appsMarketPlaceInfo.applicationInformations();
    case AppsMarketPlaceModel::HasPricingPlans:
        return appsMarketPlaceInfo.isPaid();
    case AppsMarketPlaceModel::RequestedApps:
        return appsMarketPlaceInfo.requested();
    case AppsMarketPlaceModel::Private:
        return appsMarketPlaceInfo.installedInfo().isPrivate();
    case AppsMarketPlaceModel::Migrated:
        return appsMarketPlaceInfo.installedInfo().migrated();
    case AppsMarketPlaceModel::Installed:
        return appsMarketPlaceInfo.installedInfo().isValid();
    case AppsMarketPlaceModel::Status:
        return QVariant::fromValue(appsMarketPlaceInfo.installedInfo().status());
    case AppsMarketPlaceModel::ApplicationPermissions:
        return QVariant::fromValue(appsMarketPlaceInfo.permissionsDescription());
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
