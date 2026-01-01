/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/appscountinfo.h"
#include "apps/appsmarketplaceinfo.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AppsMarketPlaceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AppsCategoriesRoles {
        AppName = Qt::UserRole + 1,
        AppId,
        AppVersion,
        Categories,
        Price,
        HasPricingPlans,
        Description,
        ShortDescription,
        PurchaseType,
        DocumentationUrl,
        IsEnterpriseOnly,
        Pixmap,
        ModifiedDate,
        ApplicationInformations,
        RequestedApps,
        Private,
        Migrated,
        Installed,
        Status,
        ApplicationPermissions,
    };
    Q_ENUM(AppsCategoriesRoles)

    explicit AppsMarketPlaceModel(QObject *parent = nullptr);
    ~AppsMarketPlaceModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QList<AppsMarketPlaceInfo> appsMarketPlaceInfos() const;
    void setAppsMarketPlaceInfos(const QList<AppsMarketPlaceInfo> &appsCategories);

    [[nodiscard]] bool wasFilled() const;

    void clear();

    [[nodiscard]] AppsCountInfo appsCountInfo() const;
    void setAppsCountInfo(const AppsCountInfo &newAppsCountInfo);

    void addInstalledInfo(const AppsMarketPlaceInstalledInfo &info);

    void clearInstalledAppsInformation();

    void removeApp(const QString &appId);

    void updateAppStatus(const QString &appId, const QString &status);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString appName(const AppsMarketPlaceInfo &appsMarketPlaceInfo) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QByteArray appId(const AppsMarketPlaceInfo &appsMarketPlaceInfo) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QPixmap pixmap(const AppsMarketPlaceInfo &appsMarketPlaceInfo) const;
    QList<AppsMarketPlaceInfo> mAppsMarketPlaceInfos;
    AppsCountInfo mAppsCountInfo;
};
