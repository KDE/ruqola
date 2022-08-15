/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryusersproxymodel.h"
#include "directoryusersmodel.h"

DirectoryUsersProxyModel::DirectoryUsersProxyModel(QAbstractListModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
}

DirectoryUsersProxyModel::~DirectoryUsersProxyModel() = default;

bool DirectoryUsersProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == DirectoryUsersModel::JoinAt) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DirectoryUsersModel::JoinAtDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DirectoryUsersModel::JoinAtDateTime);
        return SearchTreeBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return SearchTreeBaseFilterProxyModel::lessThan(left, right);
}
