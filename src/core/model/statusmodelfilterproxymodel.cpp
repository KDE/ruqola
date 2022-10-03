/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "statusmodelfilterproxymodel.h"
#include "statusmodel.h"

StatusModelFilterProxyModel::StatusModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

StatusModelFilterProxyModel::~StatusModelFilterProxyModel() = default;

bool StatusModelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const int leftString = sourceModel()->data(left, StatusModel::Order).toInt();
        const int rightString = sourceModel()->data(right, StatusModel::Order).toInt();
        return rightString < leftString;
    } else {
        return false;
    }
    return true;
}

bool StatusModelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    if (mUseOnlyStandardStatus) {
        const int order = sourceIndex.data(StatusModel::Order).toInt();
        return order > 10; // Standard entry has order > 10
    }
    if (!mAllowOfflineSupport) {
        const User::PresenceStatus status = sourceIndex.data(StatusModel::Status).value<User::PresenceStatus>();
        if (status == User::PresenceStatus::PresenceOffline) {
            return false;
        }
    }
    return true;
}

bool StatusModelFilterProxyModel::allowOfflineSupport() const
{
    return mAllowOfflineSupport;
}

void StatusModelFilterProxyModel::setAllowOfflineSupport(bool newAllowOfflineSupport)
{
    if (mAllowOfflineSupport != newAllowOfflineSupport) {
        mAllowOfflineSupport = newAllowOfflineSupport;
        invalidateFilter();
    }
}

bool StatusModelFilterProxyModel::useOnlyStandardStatus() const
{
    return mUseOnlyStandardStatus;
}

void StatusModelFilterProxyModel::setUseOnlyStandardStatus(bool useOnlyStandardStatus)
{
    mUseOnlyStandardStatus = useOnlyStandardStatus;
}
