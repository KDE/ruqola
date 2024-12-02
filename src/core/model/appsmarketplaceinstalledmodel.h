/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/appscountinfo.h"
#include "apps/appsmarketplaceinstalledinfo.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AppsMarketPlaceInstalledModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AppsCategoriesRoles {
        AppName = Qt::UserRole + 1,
        AppId,
        AppVersion,
    };
    Q_ENUM(AppsCategoriesRoles)

    explicit AppsMarketPlaceInstalledModel(QObject *parent = nullptr);
    ~AppsMarketPlaceInstalledModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QList<AppsMarketPlaceInstalledInfo> appsCategories() const;
    void setAppsCategories(const QList<AppsMarketPlaceInstalledInfo> &appsCategories);

    [[nodiscard]] bool wasFilled() const;

    void clear();

private:
    QList<AppsMarketPlaceInstalledInfo> mAppsMarketPlaceInstalledInfos;
};
