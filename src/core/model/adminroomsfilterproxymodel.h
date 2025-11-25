/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "searchtreebasefilterproxymodel.h"
class CustomBaseModel;
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
    Q_DECLARE_FLAGS(FilterRooms, FilterRoom)
    Q_FLAG(FilterRooms)

    explicit AdminRoomsFilterProxyModel(CustomBaseModel *model = nullptr, QObject *parent = nullptr);
    ~AdminRoomsFilterProxyModel() override;

    void setFilterRooms(AdminRoomsFilterProxyModel::FilterRooms filters);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    AdminRoomsFilterProxyModel::FilterRooms mFilters = FilterRoom::All;
};
