/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchataccountfilterproxymodel.h"
#include "rocketchataccountmodel.h"
#include "ruqola_plasma_activities_debug.h"
#if HAVE_ACTIVITY_SUPPORT
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
#if HAVE_ACTIVITY_SUPPORT
    if (mFilterActivities) {
        if (mActivitiesManager && mActivitiesManager->enabled()) {
            const auto activities = sourceModel()->index(source_row, 0).data(RocketChatAccountModel::Activities).toStringList();
            const auto activitiesEnabled = sourceModel()->index(source_row, 0).data(RocketChatAccountModel::ActivitiesEnabled).toBool();
            if (activitiesEnabled) {
                if (!activities.isEmpty()) {
                    return mActivitiesManager->isInCurrentActivity(activities);
                } else {
                    return false;
                }
            }
        }
    }
#endif
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool RocketChatAccountFilterProxyModel::filterActivities() const
{
    return mFilterActivities;
}

void RocketChatAccountFilterProxyModel::setFilterActivities(bool newFilterActivities)
{
    if (mFilterActivities != newFilterActivities) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        mFilterActivities = newFilterActivities;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateFilter();
#endif
    }
}

void RocketChatAccountFilterProxyModel::slotActivitiesChanged()
{
#if HAVE_ACTIVITY_SUPPORT
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    beginFilterChange();
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
    invalidateFilter();
#endif
#endif
}

#if HAVE_ACTIVITY_SUPPORT
ActivitiesManager *RocketChatAccountFilterProxyModel::activitiesManager() const
{
    return mActivitiesManager;
}

void RocketChatAccountFilterProxyModel::setActivitiesManager(ActivitiesManager *newActivitiesManager)
{
    mActivitiesManager = newActivitiesManager;
    if (mActivitiesManager) {
        connect(mActivitiesManager, &ActivitiesManager::activitiesChanged, this, [this]() {
            qCDebug(RUQOLA_PLASMAACTIVITIES_LOG) << " invalidate filter";
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
            beginFilterChange();
            endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
            invalidateFilter();
#endif
        });
    }
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
