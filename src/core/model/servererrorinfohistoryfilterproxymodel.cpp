/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistoryfilterproxymodel.h"
#include "config-ruqola.h"
#include "servererrorinfohistorymodel.h"
#if HAVE_TEXT_UTILS
#include <TextUtils/ConvertText>
#endif

ServerErrorInfoHistoryFilterProxyModel::ServerErrorInfoHistoryFilterProxyModel(QObject *parent)
    : SortFilterProxyModelBase{parent}
{
}

ServerErrorInfoHistoryFilterProxyModel::~ServerErrorInfoHistoryFilterProxyModel() = default;

bool ServerErrorInfoHistoryFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);

    if (!mAccountNameFilter.isEmpty()) {
        if (!modelIndex.data(ServerErrorInfoHistoryModel::AccountName).toString().contains(mAccountNameFilter, Qt::CaseInsensitive)) {
            return false;
        }
    }
    auto match = [&](int role) {
        return mFilterString.isEmpty() || modelIndex.data(role).toString().contains(mFilterString, Qt::CaseInsensitive);
    };
    if (!match(ServerErrorInfoHistoryModel::AccountName) && !match(ServerErrorInfoHistoryModel::MessageStr)) {
        return false;
    }
    return true;
}

void ServerErrorInfoHistoryFilterProxyModel::setAccountNameFilter(const QString &newAccountNameFilter)
{
    if (mAccountNameFilter != newAccountNameFilter) {
        mAccountNameFilter = newAccountNameFilter;
        invalidate();
    }
}

#include "moc_servererrorinfohistoryfilterproxymodel.cpp"
