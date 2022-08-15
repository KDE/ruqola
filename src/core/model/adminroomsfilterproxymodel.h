/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "searchtreebasefilterproxymodel.h"
class DirectoryBaseModel;
class LIBRUQOLACORE_EXPORT AdminRoomsFilterProxyModel : public SearchTreeBaseFilterProxyModel
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
