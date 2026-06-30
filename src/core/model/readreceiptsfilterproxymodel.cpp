/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "readreceiptsfilterproxymodel.h"
#include "readreceiptsmodel.h"

ReadReceiptsFilterProxyModel::ReadReceiptsFilterProxyModel(QObject *parent)
    : SortFilterProxyModelBase{parent}
{
}

ReadReceiptsFilterProxyModel::~ReadReceiptsFilterProxyModel() = default;

bool ReadReceiptsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mFilterString.isEmpty()) {
        return true;
    }

    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);
    auto match = [&](int role) {
        return contains(modelIndex.data(role).toString());
    };
    if (!match(ReadReceiptsModel::UserName) && !match(ReadReceiptsModel::Name)) {
        return false;
    }

    return true;
}

#include "moc_readreceiptsfilterproxymodel.cpp"
