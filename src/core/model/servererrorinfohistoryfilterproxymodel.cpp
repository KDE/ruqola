/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistoryfilterproxymodel.h"
#include "servererrorinfohistorymodel.h"

ServerErrorInfoHistoryFilterProxyModel::ServerErrorInfoHistoryFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

ServerErrorInfoHistoryFilterProxyModel::~ServerErrorInfoHistoryFilterProxyModel() = default;

void ServerErrorInfoHistoryFilterProxyModel::setFilterString(const QString &string)
{
    mFilterString = string;
    invalidate();
}

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
