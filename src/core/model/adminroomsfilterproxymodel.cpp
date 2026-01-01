/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsfilterproxymodel.h"

#include "adminroomsmodel.h"
using namespace Qt::Literals::StringLiterals;
AdminRoomsFilterProxyModel::AdminRoomsFilterProxyModel(CustomBaseModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    sort(0, Qt::DescendingOrder);
}

AdminRoomsFilterProxyModel::~AdminRoomsFilterProxyModel() = default;

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
    const QModelIndex sourceChannelTypeIndex = sourceModel()->index(source_row, AdminRoomsModel::AdminRoomsRoles::ChannelType, source_parent);
    const QString channelType = sourceModel()->data(sourceChannelTypeIndex).toString();
    if (mFilters & FilterRoom::DirectRooms) {
        if (channelType == "d"_L1) {
            return true;
        }
    }
    if (mFilters & FilterRoom::PublicRooms) {
        if (channelType == "c"_L1) {
            return true;
        }
    }
    if (mFilters & FilterRoom::PrivateRooms) {
        if (channelType == "p"_L1) {
            return true;
        }
    }
    if (mFilters & FilterRoom::Omnichannel) {
        if (channelType == "l"_L1) {
            return true;
        }
    }
    if (mFilters & FilterRoom::DiscussionRooms) {
        const QModelIndex sourceIndexIsDiscussion = sourceModel()->index(source_row, AdminRoomsModel::AdminRoomsRoles::IsDiscussion, source_parent);
        const bool isDiscussion = sourceModel()->data(sourceIndexIsDiscussion).toBool();
        return isDiscussion;
    }

    if (mFilters & FilterRoom::TeamRooms) {
        const QModelIndex sourceIndexIsTeam = sourceModel()->index(source_row, AdminRoomsModel::AdminRoomsRoles::IsTeam, source_parent);
        const bool isTeam = sourceModel()->data(sourceIndexIsTeam).toBool();
        return isTeam;
    }
    return false;
}

#include "moc_adminroomsfilterproxymodel.cpp"
