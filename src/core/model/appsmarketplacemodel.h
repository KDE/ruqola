/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

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
        PrivateApps,
        RequestedApps,
        InstalledApps,
    };
    Q_ENUM(AppsCategoriesRoles)

    explicit AppsMarketPlaceModel(QObject *parent = nullptr);
    ~AppsMarketPlaceModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QList<AppsMarketPlaceInfo> appsCategories() const;
    void setAppsCategories(const QList<AppsMarketPlaceInfo> &appsCategories);

    [[nodiscard]] bool wasFilled() const;

    void clear();

    [[nodiscard]] AppsCountInfo appsCountInfo() const;
    void setAppsCountInfo(const AppsCountInfo &newAppsCountInfo);

private:
    QList<AppsMarketPlaceInfo> mAppsMarketPlaceInfos;
    AppsCountInfo mAppsCountInfo;
};
