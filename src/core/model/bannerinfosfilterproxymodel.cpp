/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannerinfosfilterproxymodel.h"
#include "bannerinfosmodel.h"

BannerInfosFilterProxyModel::BannerInfosFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

BannerInfosFilterProxyModel::~BannerInfosFilterProxyModel() = default;

bool BannerInfosFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
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

bool BannerInfosFilterProxyModel::showUnread() const
{
    return mShowUnread;
}

void BannerInfosFilterProxyModel::setShowUnread(bool newShowUnread)
{
    if (mShowUnread != newShowUnread) {
        mShowUnread = newShowUnread;
        invalidateFilter();
    }
}
