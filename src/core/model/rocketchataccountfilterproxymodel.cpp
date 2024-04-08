/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountfilterproxymodel.h"
#include "config-ruqola.h"
#include "rocketchataccountmodel.h"
#if HAS_ACTIVITY_SUPPORT
#include "activities/activitiesmanager.h"
#endif

RocketChatAccountFilterProxyModel::RocketChatAccountFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(RocketChatAccountModel::Name);
    sort(0);
}

RocketChatAccountFilterProxyModel::~RocketChatAccountFilterProxyModel() = default;

bool RocketChatAccountFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
#if HAS_ACTIVITY_SUPPORT
    if (mActivitiesManager && mActivitiesManager->enabled()) {
        const auto activities = sourceModel()->index(source_row, 0).data(RocketChatAccountModel::Activities).toStringList();
        // TODO verify
        return mActivitiesManager->isInCurrentActivity(activities);
    }
#endif
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

#if HAS_ACTIVITY_SUPPORT
ActivitiesManager *RocketChatAccountFilterProxyModel::activitiesManager() const
{
    return mActivitiesManager;
}

void RocketChatAccountFilterProxyModel::setActivitiesManager(ActivitiesManager *newActivitiesManager)
{
    mActivitiesManager = newActivitiesManager;
}
#endif

bool RocketChatAccountFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (!mAccountOrder.isEmpty()) {
        if (left.isValid() && right.isValid()) {
            const QString leftString = sourceModel()->data(left, RocketChatAccountModel::AccountName).toString();
            const QString rightString = sourceModel()->data(right, RocketChatAccountModel::AccountName).toString();
            const int leftAccountIndex = mAccountOrder.indexOf(leftString);
            const int rightAccountIndex = mAccountOrder.indexOf(rightString);
            if (leftAccountIndex < rightAccountIndex) {
                return true;
            } else if (leftAccountIndex > rightAccountIndex) {
                return false;
            }
        }
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

QStringList RocketChatAccountFilterProxyModel::accountOrder() const
{
    return mAccountOrder;
}

void RocketChatAccountFilterProxyModel::setAccountOrder(const QStringList &newAccountOrder)
{
    if (mAccountOrder != newAccountOrder) {
        mAccountOrder = newAccountOrder;
        invalidate();
    }
}

#include "moc_rocketchataccountfilterproxymodel.cpp"
