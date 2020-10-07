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
    sort(0, Qt::DescendingOrder);
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

void AdminRoomsFilterProxyModel::setFilterRooms(AdminRoomsFilterProxyModel::FilterRooms filters)
{
    mFilters = filters;
    invalidate();
}

bool AdminRoomsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mFilters & FilterRoom::None) {
        return true;
    }

    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString channelType = sourceIndex.data(static_cast<int>(AdminRoomsModel::AdminRoomsRoles::ChannelType)).toString();
    qDebug() << "channelType " << channelType;
    qDebug() << " mFilters " << mFilters;
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
    if (mFilters & FilterRoom::DiscussionRooms) {
        if (channelType == QLatin1String("d")) { // ????? no idea.
            return true;
        }
    }
    return false;
}
