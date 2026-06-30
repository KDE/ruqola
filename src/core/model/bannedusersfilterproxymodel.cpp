/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannedusersfilterproxymodel.h"
#include "bannedusersmodel.h"
BannedUsersFilterProxyModel::BannedUsersFilterProxyModel(QObject *parent)
    : SortFilterProxyModelBase{parent}
{
}

BannedUsersFilterProxyModel::~BannedUsersFilterProxyModel() = default;

bool BannedUsersFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mFilterString.isEmpty()) {
        return true;
    }

    const QModelIndex modelIndex = sourceModel()->index(source_row, 0, source_parent);
    auto match = [&](int role) {
        return contains(modelIndex.data(role).toString());
    };
    if (!match(BannedUsersModel::UserName) && !match(BannedUsersModel::Name)) {
        return false;
    }

    return true;
}
#include "moc_bannedusersfilterproxymodel.cpp"
