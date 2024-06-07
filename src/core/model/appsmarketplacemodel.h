/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "apps/appscategoryinfo.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT AppsMarketPlaceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AppsCategoriesRoles {
        Identifier,
        Title,
        Hidden,
        LastColumn = Hidden,
    };
    Q_ENUM(AppsCategoriesRoles)

    explicit AppsMarketPlaceModel(QObject *parent = nullptr);
    ~AppsMarketPlaceModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QList<AppsCategoryInfo> appsCategories() const;
    void setAppsCategories(const QList<AppsCategoryInfo> &appsCategories);

private:
    QList<AppsCategoryInfo> mAppsCategories;
};
