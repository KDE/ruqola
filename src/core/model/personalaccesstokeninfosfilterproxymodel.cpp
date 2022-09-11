/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokeninfosfilterproxymodel.h"
#include "personalaccesstokeninfosmodel.h"

PersonalAccessTokenInfosFilterProxyModel::PersonalAccessTokenInfosFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(PersonalAccessTokenInfosModel::Name);
    sort(0);
}

PersonalAccessTokenInfosFilterProxyModel::~PersonalAccessTokenInfosFilterProxyModel() = default;

bool PersonalAccessTokenInfosFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    const int leftColumn{left.column()};
    if (leftColumn == PersonalAccessTokenInfosModel::CreateAt) {
        const QModelIndex leftMessageModelIndex = sourceModel()->index(left.row(), PersonalAccessTokenInfosModel::CreateAtDateTime);
        const QModelIndex rightMessageModelIndex = sourceModel()->index(right.row(), PersonalAccessTokenInfosModel::CreateAtDateTime);
        return lessThan(leftMessageModelIndex, rightMessageModelIndex);
    }
    return QSortFilterProxyModel::lessThan(left, right);
}
