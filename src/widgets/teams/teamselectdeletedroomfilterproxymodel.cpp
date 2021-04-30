/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "teamselectdeletedroomfilterproxymodel.h"

#include "model/teamroomsmodel.h"

TeamSelectDeletedRoomFilterProxyModel::TeamSelectDeletedRoomFilterProxyModel(TeamRoomsModel *teamModel, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mTeamRoomsModel(teamModel)
{
    setSourceModel(mTeamRoomsModel);
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    // Filter on filename for the moment
    setFilterRole(TeamRoomsModel::Name);

    setSortRole(TeamRoomsModel::Name);
    sort(0, Qt::DescendingOrder);
}

TeamSelectDeletedRoomFilterProxyModel::~TeamSelectDeletedRoomFilterProxyModel()
{
}

void TeamSelectDeletedRoomFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

Qt::ItemFlags TeamSelectDeletedRoomFilterProxyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    return Qt::ItemIsUserCheckable | defaultFlags;
}
