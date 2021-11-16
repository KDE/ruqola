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

#pragma once

#include "directorybasefilterproxymodel.h"
#include "libruqolacore_export.h"
class DirectoryBaseModel;
class LIBRUQOLACORE_EXPORT AdminRoomsFilterProxyModel : public DirectoryBaseFilterProxyModel
{
    Q_OBJECT
public:
    enum FilterRoom {
        None = 0,
        DirectRooms = 1,
        PublicRooms = 2,
        PrivateRooms = 4,
        DiscussionRooms = 8,
        TeamRooms = 16,
        Omnichannel = 32,
        All = DirectRooms | PublicRooms | DiscussionRooms | PrivateRooms | TeamRooms | Omnichannel,
    };
    Q_FLAGS(FilterRoom FilterRooms)
    Q_DECLARE_FLAGS(FilterRooms, FilterRoom)

    explicit AdminRoomsFilterProxyModel(DirectoryBaseModel *model = nullptr, QObject *parent = nullptr);
    ~AdminRoomsFilterProxyModel() override;


    void setFilterRooms(AdminRoomsFilterProxyModel::FilterRooms filters);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    AdminRoomsFilterProxyModel::FilterRooms mFilters = FilterRoom::All;
};

