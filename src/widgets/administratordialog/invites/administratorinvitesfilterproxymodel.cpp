/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "administratorinvitesfilterproxymodel.h"
#include "model/admininvitemodel.h"

AdministratorInvitesFilterProxyModel::AdministratorInvitesFilterProxyModel(QAbstractListModel *model, QObject *parent)
    : DirectoryBaseFilterProxyModel(model, parent)
{
}

AdministratorInvitesFilterProxyModel::~AdministratorInvitesFilterProxyModel() = default;

bool AdministratorInvitesFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const int leftColumn{left.column()};
    if (leftColumn == AdminInviteModel::CreateStr) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), AdminInviteModel::Create);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), AdminInviteModel::Create);
        return DirectoryBaseFilterProxyModel::lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return DirectoryBaseFilterProxyModel::lessThan(left, right);
}
