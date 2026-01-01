/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QSortFilterProxyModel>

class LIBRUQOLACORE_EXPORT AppsMarketPlaceFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum Status : uint8_t {
        AllStatus,
        Enabled,
        Disabled,
        UnknownStatus,
    };
    Q_ENUM(Status);

    enum Price : uint8_t {
        AllPrice,
        Free,
        Paid,
        Premium,
        UnknownPrice,
    };
    Q_ENUM(Price);

    enum Sorting : uint8_t {
        AtoZ,
        ZtoA,
        MostRecent,
        LeastRecent,
        UnknownSorting,
    };
    Q_ENUM(Sorting);

    struct LIBRUQOLACORE_EXPORT FilterInfo {
        QStringList categories;
        QString text;
        Status status = Status::UnknownStatus;
        Price price = Price::UnknownPrice;
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

    [[nodiscard]] bool installed() const;
    void setInstalled(bool newInstalled);

    [[nodiscard]] bool isPrivate() const;
    void setIsPrivate(bool newPrivate);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    FilterInfo mFilterInfo;
    Sorting mSorting = Sorting::AtoZ;
    bool mRequested = false;
    bool mInstalled = false;
    bool mPrivate = false;
};
Q_DECLARE_METATYPE(AppsMarketPlaceFilterProxyModel::FilterInfo)
Q_DECLARE_TYPEINFO(AppsMarketPlaceFilterProxyModel::FilterInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AppsMarketPlaceFilterProxyModel::FilterInfo &t);
