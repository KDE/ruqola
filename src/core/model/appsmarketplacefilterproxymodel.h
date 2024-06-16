/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT AppsMarketPlaceFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    struct LIBRUQOLACORE_EXPORT FilterInfo {
        QStringList categories;
        QString text;
        QString status;
        QString price;
        [[nodiscard]] bool operator==(const FilterInfo &other) const;
        [[nodiscard]] bool operator!=(const FilterInfo &other) const;
    };

    explicit AppsMarketPlaceFilterProxyModel(QObject *parent = nullptr);
    ~AppsMarketPlaceFilterProxyModel() override;

    [[nodiscard]] FilterInfo filterInfo() const;
    void setFilterInfo(const FilterInfo &newFilterInfo);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    FilterInfo mFilterInfo;
};
Q_DECLARE_METATYPE(AppsMarketPlaceFilterProxyModel::FilterInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceFilterProxyModel::FilterInfo, Q_RELOCATABLE_TYPE);
