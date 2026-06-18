/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannedusersfilterproxymodel.h"

BannedUsersFilterProxyModel::BannedUsersFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

BannedUsersFilterProxyModel::~BannedUsersFilterProxyModel() = default;

void BannedUsersFilterProxyModel::setFilterString(const QString &string)
{
    mFilterString = string;
    invalidate();
}
#include "moc_bannedusersfilterproxymodel.cpp"
