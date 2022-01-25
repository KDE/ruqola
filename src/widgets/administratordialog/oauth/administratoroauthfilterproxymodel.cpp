/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthfilterproxymodel.h"
#include "model/adminoauthmodel.h"

AdministratorOauthFilterProxyModel::AdministratorOauthFilterProxyModel(QAbstractListModel *model, QObject *parent)
    : DirectoryBaseFilterProxyModel(model, parent)
{
}

AdministratorOauthFilterProxyModel::~AdministratorOauthFilterProxyModel() = default;

bool AdministratorOauthFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == AdminOauthModel::CreateStr) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), AdminOauthModel::Create);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), AdminOauthModel::Create);
        return DirectoryBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return DirectoryBaseFilterProxyModel::lessThan(left, right);
}
