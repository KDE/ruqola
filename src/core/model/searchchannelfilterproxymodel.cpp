/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchchannelfilterproxymodel.h"
#include "searchchannelmodel.h"

SearchChannelFilterProxyModel::SearchChannelFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(SearchChannelModel::ChannelName);
    sort(0);
}

SearchChannelFilterProxyModel::~SearchChannelFilterProxyModel() = default;

bool SearchChannelFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    if (left.isValid() && right.isValid()) {
        const QString leftString = sourceModel()->data(left, SearchChannelModel::ChannelName).toString();
        const QString rightString = sourceModel()->data(right, SearchChannelModel::ChannelName).toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    } else {
        return false;
    }
}

#include "moc_searchchannelfilterproxymodel.cpp"
