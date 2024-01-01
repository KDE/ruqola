/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamroomsfilterproxymodel.h"
#include "teamroomsmodel.h"

TeamRoomsFilterProxyModel::TeamRoomsFilterProxyModel(TeamRoomsModel *teamModel, QObject *parent)
    : QSortFilterProxyModel(parent)
    , mTeamRoomsModel(teamModel)
{
    setSourceModel(mTeamRoomsModel);

    setFilterCaseSensitivity(Qt::CaseInsensitive);
    // Filter on filename for the moment
    setFilterRole(TeamRoomsModel::Name);

    setSortRole(TeamRoomsModel::Name);
    sort(0, Qt::AscendingOrder);
}

TeamRoomsFilterProxyModel::~TeamRoomsFilterProxyModel() = default;

void TeamRoomsFilterProxyModel::setFilterString(const QString &string)
{
    setFilterFixedString(string);
}

bool TeamRoomsFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (!mSortByAutoJoin) {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const bool autojoin = sourceIndex.data(TeamRoomsModel::AutoJoin).toBool();
    return autojoin && QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool TeamRoomsFilterProxyModel::sortByAutoJoin() const
{
    return mSortByAutoJoin;
}

void TeamRoomsFilterProxyModel::setSortByAutoJoin(bool sortByAutoJoin)
{
    if (mSortByAutoJoin != sortByAutoJoin) {
        mSortByAutoJoin = sortByAutoJoin;
        invalidateFilter();
    }
}

#include "moc_teamroomsfilterproxymodel.cpp"
