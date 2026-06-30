/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannedusersfilterproxymodel.h"
#include "bannedusersmodel.h"
#include "config-ruqola.h"
#if HAVE_TEXT_UTILS
#include <TextUtils/ConvertText>
#endif
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
#if HAVE_TEXT_UTILS
        const QString str = TextUtils::ConvertText::normalize(modelIndex.data(role).toString());
        return str.contains(mFilterString, Qt::CaseInsensitive);
#else
        const QString str = modelIndex.data(role).toString();
        return str.contains(mFilterString, Qt::CaseInsensitive);
#endif
    };
    if (!match(BannedUsersModel::UserName) && !match(BannedUsersModel::Name)) {
        return false;
    }

    return true;
}
#include "moc_bannedusersfilterproxymodel.cpp"
