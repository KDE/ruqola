/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorymodelfilterproxymodel.h"
#include "notificationhistorymodel.h"

NotificationHistoryModelFilterProxyModel::NotificationHistoryModelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

NotificationHistoryModelFilterProxyModel::~NotificationHistoryModelFilterProxyModel() = default;

void NotificationHistoryModelFilterProxyModel::setFilterString(const QString &string)
{
    mFilterString = string;
    invalidate();
}

bool NotificationHistoryModelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);

    auto match = [&](int role) {
        return mFilterString.isEmpty() || modelIndex.data(role).toString().contains(mFilterString, Qt::CaseInsensitive);
    };
    if (!match(NotificationHistoryModel::RoomName) && !match(NotificationHistoryModel::AccountName) && !match(NotificationHistoryModel::SenderName)
        && !match(NotificationHistoryModel::MessageStr)) {
        return false;
    }
    return true;
}

void NotificationHistoryModelFilterProxyModel::setAccountNameFilter(const QString &newAccountNameFilter)
{
    if (mAccountNameFilter != newAccountNameFilter) {
        mAccountNameFilter = newAccountNameFilter;
        invalidate();
    }
}
