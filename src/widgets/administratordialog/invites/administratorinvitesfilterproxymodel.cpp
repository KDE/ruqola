/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorinvitesfilterproxymodel.h"
#include "model/admininvitemodel.h"

AdministratorInvitesFilterProxyModel::AdministratorInvitesFilterProxyModel(QAbstractListModel *model, QObject *parent)
    : SearchTreeBaseFilterProxyModel(model, parent)
{
}

AdministratorInvitesFilterProxyModel::~AdministratorInvitesFilterProxyModel() = default;

bool AdministratorInvitesFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == AdminInviteModel::CreateAtStr) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), AdminInviteModel::CreateAt);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), AdminInviteModel::CreateAt);
        return SearchTreeBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return SearchTreeBaseFilterProxyModel::lessThan(left, right);
}
