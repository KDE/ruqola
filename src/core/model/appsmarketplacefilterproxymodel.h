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
    enum Status {
        AllStatus,
        Enabled,
        Disabled,
        UnknownStatus,
    };
    Q_ENUM(Status);

    enum Price {
        AllPrice,
        Free,
        Paid,
        Premium,
        UnknownPrice,
    };
    Q_ENUM(Price);

    enum Sorting {
        AtoZ,
        ZtoA,
        MostRecent,
        LeastRecent,
        UnknownSorting,
    };
    Q_ENUM(Sorting);

    enum InstalledApps {
        AnyInfoApps,
        PrivateApps,
        AllInstalledApps,
    };
    Q_ENUM(InstalledApps);

    struct LIBRUQOLACORE_EXPORT FilterInfo {
        QStringList categories;
        QString text;
        Status status = Status::UnknownStatus;
        Price price = Price::UnknownPrice;
        InstalledApps installedApps = InstalledApps::AllInstalledApps;
        [[nodiscard]] bool operator==(const FilterInfo &other) const;
        [[nodiscard]] bool operator!=(const FilterInfo &other) const;
    };

    explicit AppsMarketPlaceFilterProxyModel(QObject *parent = nullptr);
    ~AppsMarketPlaceFilterProxyModel() override;

    [[nodiscard]] FilterInfo filterInfo() const;
    void setFilterInfo(const FilterInfo &newFilterInfo);

    [[nodiscard]] Sorting sorting() const;
    void setSorting(Sorting newSorting);

    [[nodiscard]] bool requested() const;
    void setRequested(bool newRequested);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    FilterInfo mFilterInfo;
    Sorting mSorting = Sorting::AtoZ;
    bool mRequested = false;
};
Q_DECLARE_METATYPE(AppsMarketPlaceFilterProxyModel::FilterInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceFilterProxyModel::FilterInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceFilterProxyModel::FilterInfo &t);
