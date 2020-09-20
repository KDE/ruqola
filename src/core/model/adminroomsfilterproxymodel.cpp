/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "adminroomsfilterproxymodel.h"
#include "adminroomsmodel.h"
AdminRoomsFilterProxyModel::AdminRoomsFilterProxyModel(AdminRoomsModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mAdminRoomsModel(model)
{
    setSourceModel(mAdminRoomsModel);
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
//    setFilterRole(DiscussionsModel::Description);
//    setSortRole(DiscussionsModel::SortByTimeStamp);
    sort(0, Qt::DescendingOrder);
//    connect(mAdminRoomsModel, &AdminRoomsModel::hasFullListChanged, this, &AdminRoomsFilterProxyModel::hasFullListChanged);
//    connect(mAdminRoomsModel, &AdminRoomsModel::loadingInProgressChanged, this, &AdminRoomsFilterProxyModel::loadingInProgressChanged);
}

AdminRoomsFilterProxyModel::~AdminRoomsFilterProxyModel()
{
}

QHash<int, QByteArray> AdminRoomsFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel()) {
        return source->roleNames();
    }
    return QHash<int, QByteArray>();
}

void AdminRoomsFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

void AdminRoomsFilterProxyModel::clearFilter()
{
    setFilterFixedString(QString());
}

//int AdminRoomsFilterProxyModel::total() const
//{
//    return mAdminRoomsModel->total();
//}

//bool AdminRoomsFilterProxyModel::hasFullList() const
//{
//    return mAdminRoomsModel->hasFullList();
//}

//bool AdminRoomsFilterProxyModel::loadMoreAdminRoomsInProgress() const
//{
//    return mAdminRoomsModel->loadMoreDiscussionsInProgress();
//}
