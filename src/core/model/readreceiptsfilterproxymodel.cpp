/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "readreceiptsfilterproxymodel.h"
#include "bannerinfosmodel.h"

ReadReceiptsFilterProxyModel::ReadReceiptsFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

ReadReceiptsFilterProxyModel::~ReadReceiptsFilterProxyModel() = default;

bool ReadReceiptsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!mShowUnread) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const bool isRead = sourceIndex.data(BannerInfosModel::Read).toBool();
    if (!isRead) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    return false;
}

bool ReadReceiptsFilterProxyModel::showUnread() const
{
    return mShowUnread;
}

void ReadReceiptsFilterProxyModel::setShowUnread(bool newShowUnread)
{
    if (mShowUnread != newShowUnread) {
        beginFilterChange();
        mShowUnread = newShowUnread;
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
    }
}

#include "moc_readreceiptsfilterproxymodel.cpp"
