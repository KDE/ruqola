/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplacefilterproxymodel.h"

AppsMarketPlaceFilterProxyModel::AppsMarketPlaceFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

AppsMarketPlaceFilterProxyModel::~AppsMarketPlaceFilterProxyModel() = default;

bool AppsMarketPlaceFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // TODO
    return QSortFilterProxyModel::filterAcceptsColumn(source_row, source_parent);
}

AppsMarketPlaceFilterProxyModel::FilterInfo AppsMarketPlaceFilterProxyModel::filterInfo() const
{
    return mFilterInfo;
}

void AppsMarketPlaceFilterProxyModel::setFilterInfo(const FilterInfo &newFilterInfo)
{
    mFilterInfo = newFilterInfo;
}

#include "moc_appsmarketplacefilterproxymodel.cpp"
