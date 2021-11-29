/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directoryroomsproxymodel.h"
#include "directoryroomsmodel.h"

DirectoryRoomsProxyModel::DirectoryRoomsProxyModel(QAbstractListModel *model, QObject *parent)
    : DirectoryBaseFilterProxyModel(model, parent)
{
}

DirectoryRoomsProxyModel::~DirectoryRoomsProxyModel() = default;

bool DirectoryRoomsProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == DirectoryRoomsModel::LastMessageStr) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DirectoryRoomsModel::LastMessageDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DirectoryRoomsModel::LastMessageDateTime);
        return DirectoryBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    } else if (leftColumn == DirectoryRoomsModel::CreatedStr) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), DirectoryRoomsModel::CreatedDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), DirectoryRoomsModel::CreatedDateTime);
        return DirectoryBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return DirectoryBaseFilterProxyModel::lessThan(left, right);
}
