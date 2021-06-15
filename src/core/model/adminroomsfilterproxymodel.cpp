/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
AdminRoomsFilterProxyModel::AdminRoomsFilterProxyModel(DirectoryBaseModel *model, QObject *parent)
    : DirectoryBaseFilterProxyModel(model, parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    sort(0, Qt::DescendingOrder);
}

AdminRoomsFilterProxyModel::~AdminRoomsFilterProxyModel()
{
}

void AdminRoomsFilterProxyModel::setFilterRooms(AdminRoomsFilterProxyModel::FilterRooms filters)
{
    if (mFilters != filters) {
        mFilters = filters;
        invalidate();
    }
}

bool AdminRoomsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent)) {
        return false;
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, AdminRoomsModel::AdminRoomsRoles::ChannelType, source_parent);
    const QString channelType = sourceModel()->data(sourceIndex).toString();
    if (mFilters & FilterRoom::DirectRooms) {
        if (channelType == QLatin1String("d")) {
            return true;
        }
    }
    if (mFilters & FilterRoom::PublicRooms) {
        if (channelType == QLatin1String("c")) {
            return true;
        }
    }
    if (mFilters & FilterRoom::PrivateRooms) {
        if (channelType == QLatin1String("p")) {
            return true;
        }
    }
    return false;
}
