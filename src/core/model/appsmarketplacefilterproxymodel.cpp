/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
    if (mRequested) {
        if (!modelIndex.data(AppsMarketPlaceModel::RequestedApps).toBool()) {
            return false;
        }
    }

    if (mInstalled) {
        if (!modelIndex.data(AppsMarketPlaceModel::Installed).toBool()) {
            return false;
        }
        // Don't show private apps
        if (modelIndex.data(AppsMarketPlaceModel::Private).toBool()) {
            return false;
        }
    }

    if (mPrivate) {
        if (!modelIndex.data(AppsMarketPlaceModel::Private).toBool()) {
            return false;
        }
    }

    if (!mFilterInfo.text.isEmpty()) {
        if (!modelIndex.data(AppsMarketPlaceModel::ShortDescription).toString().contains(mFilterInfo.text, Qt::CaseInsensitive)
            && !modelIndex.data(AppsMarketPlaceModel::AppName).toString().contains(mFilterInfo.text, Qt::CaseInsensitive)) {
            return false;
        }
    }
    if (!mFilterInfo.categories.isEmpty()) {
        const QStringList categories = modelIndex.data(AppsMarketPlaceModel::Categories).toStringList();
        if (categories.isEmpty()) {
            return true;
        }
        bool found = false;
        for (const QString &cat : std::as_const(mFilterInfo.categories)) {
            if (categories.contains(cat)) {
                found = true;
                break;
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
        if (modelIndex.data(AppsMarketPlaceModel::HasPricingPlans).toBool() || modelIndex.data(AppsMarketPlaceModel::IsEnterpriseOnly).toBool()) {
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
        }
    }
    }

    switch (mFilterInfo.status) {
    case Status::AllStatus:
        break;
    case Status::Enabled: {
        const AppsMarketPlaceInstalledInfo::Status status = modelIndex.data(AppsMarketPlaceModel::Status).value<AppsMarketPlaceInstalledInfo::Status>();
        if (status == AppsMarketPlaceInstalledInfo::Status::AutoEnabled || status == AppsMarketPlaceInstalledInfo::Status::ManuallyEnabled) {
            return true;
        } else {
            return false;
        }
    }
    case Status::Disabled: {
        const AppsMarketPlaceInstalledInfo::Status status = modelIndex.data(AppsMarketPlaceModel::Status).value<AppsMarketPlaceInstalledInfo::Status>();
        if (status == AppsMarketPlaceInstalledInfo::Status::Initialized) {
            return true;
        } else {
            return false;
        }
    }
    case Status::UnknownStatus:
        break;
    }

    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool AppsMarketPlaceFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (left.isValid() && right.isValid()) {
        switch (mSorting) {
        case Sorting::AtoZ: {
            const QString leftString = sourceModel()->data(left, AppsMarketPlaceModel::AppName).toString();
            const QString rightString = sourceModel()->data(right, AppsMarketPlaceModel::AppName).toString();
            return QString::localeAwareCompare(leftString, rightString) < 0;
        }
        case Sorting::ZtoA: {
            const QString leftString = sourceModel()->data(left, AppsMarketPlaceModel::AppName).toString();
            const QString rightString = sourceModel()->data(right, AppsMarketPlaceModel::AppName).toString();
            return QString::localeAwareCompare(leftString, rightString) > 0;
        }
        case Sorting::LeastRecent: {
            const qint64 leftDateTime = sourceModel()->data(left, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            const qint64 rightDateTime = sourceModel()->data(right, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            // qDebug() << " leftDateTime " << leftDateTime << " rightDateTime " << rightDateTime;
            return leftDateTime > rightDateTime;
        }
        case Sorting::MostRecent: {
            const qint64 leftDateTime = sourceModel()->data(left, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            const qint64 rightDateTime = sourceModel()->data(right, AppsMarketPlaceModel::ModifiedDate).toLongLong();
            // qDebug() << " leftDateTime " << leftDateTime << " rightDateTime " << rightDateTime;
            return leftDateTime < rightDateTime;
        }
        case Sorting::UnknownSorting:
            return QSortFilterProxyModel::lessThan(left, right);
        }
    } else {
        return false;
    }
    return false;
}

bool AppsMarketPlaceFilterProxyModel::isPrivate() const
{
    return mPrivate;
}

void AppsMarketPlaceFilterProxyModel::setIsPrivate(bool newPrivate)
{
    if (mPrivate != newPrivate) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        mPrivate = newPrivate;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
}

bool AppsMarketPlaceFilterProxyModel::installed() const
{
    return mInstalled;
}

void AppsMarketPlaceFilterProxyModel::setInstalled(bool newInstalled)
{
    if (mInstalled != newInstalled) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        mInstalled = newInstalled;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
}

bool AppsMarketPlaceFilterProxyModel::requested() const
{
    return mRequested;
}

void AppsMarketPlaceFilterProxyModel::setRequested(bool newRequested)
{
    if (mRequested != newRequested) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        mRequested = newRequested;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        mFilterInfo = newFilterInfo;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
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
