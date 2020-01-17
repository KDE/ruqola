/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "discussionsfilterproxymodel.h"
#include "discussionsmodel.h"
DiscussionsFilterProxyModel::DiscussionsFilterProxyModel(DiscussionsModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mDiscussionsModel(model)
{
    setSourceModel(mDiscussionsModel);
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(DiscussionsModel::Description);
    setSortRole(DiscussionsModel::SortByTimeStamp);
    sort(0, Qt::DescendingOrder);
    connect(mDiscussionsModel, &DiscussionsModel::hasFullListChanged, this, &DiscussionsFilterProxyModel::hasFullListChanged);
}

DiscussionsFilterProxyModel::~DiscussionsFilterProxyModel()
{
}

QHash<int, QByteArray> DiscussionsFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

void DiscussionsFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

void DiscussionsFilterProxyModel::clearFilter()
{
    setFilterFixedString(QString());
}

int DiscussionsFilterProxyModel::total() const
{
    return mDiscussionsModel->total();
}

bool DiscussionsFilterProxyModel::hasFullList() const
{
    return mDiscussionsModel->hasFullList();
}
