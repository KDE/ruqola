/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryteamsproxymodel.h"
#include "directoryteamsmodel.h"

DirectoryTeamsProxyModel::DirectoryTeamsProxyModel(QAbstractListModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
}

DirectoryTeamsProxyModel::~DirectoryTeamsProxyModel() = default;

bool DirectoryTeamsProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == DirectoryTeamsModel::CreateTeam) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DirectoryTeamsModel::CreateTeamDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DirectoryTeamsModel::CreateTeamDateTime);
        return SearchTreeBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return SearchTreeBaseFilterProxyModel::lessThan(left, right);
}

#include "moc_directoryteamsproxymodel.cpp"
