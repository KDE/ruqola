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
        const QStringList categories = modelIndex.data(AppsMarketPlaceModel::Categories).toStringList();
        bool found = false;
        for (const QString &cat : std::as_const(mFilterInfo.categories)) {
            if (categories.contains(cat)) {
                found = true;
            }
        }
        if (!found) {
            return false;
        }
    }
    switch (mFilterInfo.price) {
    case Price::AllPrice:
    case Price::UnknownPrice:
        break;
    case Price::Free: {
        if (modelIndex.data(AppsMarketPlaceModel::HasPricingPlans).toBool()) {
            return false;
        }
        break;
    }
    case Price::Paid: {
        if (!modelIndex.data(AppsMarketPlaceModel::HasPricingPlans).toBool()) {
            return false;
        }
        break;
    }
    case Price::Premium: {
        const bool isEnterpriseOnly = modelIndex.data(AppsMarketPlaceModel::IsEnterpriseOnly).toBool();
        if (!isEnterpriseOnly) {
            return false;
            break;
        }
    }
    }

    return QSortFilterProxyModel::filterAcceptsColumn(source_row, source_parent);
}

bool AppsMarketPlaceFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (left.isValid() && right.isValid()) {
        switch (mSorting) {
        case AtoZ: {
            const QString leftString = sourceModel()->data(left, AppsMarketPlaceModel::AppName).toString();
            const QString rightString = sourceModel()->data(right, AppsMarketPlaceModel::AppName).toString();
            return QString::localeAwareCompare(leftString, rightString) < 0;
        }
        case ZtoA: {
            const QString leftString = sourceModel()->data(left, AppsMarketPlaceModel::AppName).toString();
            const QString rightString = sourceModel()->data(right, AppsMarketPlaceModel::AppName).toString();
            return QString::localeAwareCompare(leftString, rightString) > 0;
        }
        case LeastRecent: {
            const qint64 leftDateTime = sourceModel()->data(left, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            const qint64 rightDateTime = sourceModel()->data(right, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            // qDebug() << " leftDateTime " << leftDateTime << " rightDateTime " << rightDateTime;
            return leftDateTime > rightDateTime;
        }
        case MostRecent: {
            const qint64 leftDateTime = sourceModel()->data(left, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            const qint64 rightDateTime = sourceModel()->data(right, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            // qDebug() << " leftDateTime " << leftDateTime << " rightDateTime " << rightDateTime;
            return leftDateTime < rightDateTime;
        }
        case UnknownSorting:
            return QSortFilterProxyModel::lessThan(left, right);
        }
    } else {
        return false;
    }
    return false;
}

AppsMarketPlaceFilterProxyModel::Sorting AppsMarketPlaceFilterProxyModel::sorting() const
{
    return mSorting;
}

void AppsMarketPlaceFilterProxyModel::setSorting(Sorting newSorting)
{
    if (mSorting != newSorting) {
        mSorting = newSorting;
        invalidate();
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
    return d;
}

#include "moc_appsmarketplacefilterproxymodel.cpp"
