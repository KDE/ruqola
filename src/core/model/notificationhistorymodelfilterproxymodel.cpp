/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorymodelfilterproxymodel.h"
#include "notificationhistorymodel.h"

#include "config-ruqola.h"
#if HAVE_TEXT_UTILS
#include <TextUtils/ConvertText>
#endif

NotificationHistoryModelFilterProxyModel::NotificationHistoryModelFilterProxyModel(QObject *parent)
    : SortFilterProxyModelBase{parent}
{
}

NotificationHistoryModelFilterProxyModel::~NotificationHistoryModelFilterProxyModel() = default;

bool NotificationHistoryModelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);

    if (!mAccountNameFilter.isEmpty()) {
        if (!modelIndex.data(NotificationHistoryModel::AccountName).toString().contains(mAccountNameFilter, Qt::CaseInsensitive)) {
            return false;
        }
    }
    auto match = [&](int role) {
        if (mFilterString.isEmpty()) {
            return true;
        };
#if HAVE_TEXT_UTILS
        const QString str = TextUtils::ConvertText::normalize(modelIndex.data(role).toString());
        return str.contains(mFilterString, Qt::CaseInsensitive);
#else
        const QString str = modelIndex.data(role).toString();
        return str.contains(mFilterString, Qt::CaseInsensitive);
#endif
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

#include "moc_notificationhistorymodelfilterproxymodel.cpp"
