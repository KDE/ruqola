/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplacefilterproxymodel.h"
#include "appsmarketplacemodel.h"

AppsMarketPlaceFilterProxyModel::AppsMarketPlaceFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    sort(0, Qt::AscendingOrder);
}

AppsMarketPlaceFilterProxyModel::~AppsMarketPlaceFilterProxyModel() = default;

bool AppsMarketPlaceFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);
    if (!mFilterInfo.text.isEmpty()) {
        if (!modelIndex.data(AppsMarketPlaceModel::ShortDescription).toString().contains(mFilterInfo.text, Qt::CaseInsensitive)
            && !modelIndex.data(AppsMarketPlaceModel::AppName).toString().contains(mFilterInfo.text, Qt::CaseInsensitive)) {
            return false;
        }
    }
    if (!mFilterInfo.categories.isEmpty()) {
        const QStringList categories = modelIndex.data(AppsMarketPlaceModel::ShortDescription).toStringList();
        // TODO
    }
    // TODO
    return QSortFilterProxyModel::filterAcceptsColumn(source_row, source_parent);
}

bool AppsMarketPlaceFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, AppsMarketPlaceModel::AppName).toString();
        const QString rightString = sourceModel()->data(right, AppsMarketPlaceModel::AppName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

AppsMarketPlaceFilterProxyModel::FilterInfo AppsMarketPlaceFilterProxyModel::filterInfo() const
{
    return mFilterInfo;
}

void AppsMarketPlaceFilterProxyModel::setFilterInfo(const FilterInfo &newFilterInfo)
{
    if (mFilterInfo != newFilterInfo) {
        mFilterInfo = newFilterInfo;
        invalidateFilter();
    }
}

bool AppsMarketPlaceFilterProxyModel::FilterInfo::operator==(const FilterInfo &other) const
{
    return categories == other.categories && text == other.text && other.status == status && price == other.price;
}

bool AppsMarketPlaceFilterProxyModel::FilterInfo::operator!=(const FilterInfo &other) const
{
    return !operator==(other);
}

QDebug operator<<(QDebug d, const AppsMarketPlaceFilterProxyModel::FilterInfo &t)
{
    d.space() << "categories:" << t.categories;
    d.space() << "text:" << t.text;
    d.space() << "status:" << t.status;
    d.space() << "price:" << t.price;
    d.space() << "sorting:" << t.sorting;
    return d;
}

#include "moc_appsmarketplacefilterproxymodel.cpp"
