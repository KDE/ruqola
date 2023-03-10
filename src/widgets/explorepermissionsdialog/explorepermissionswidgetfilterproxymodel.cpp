/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidgetfilterproxymodel.h"
#include "model/permissionsmodel.h"

ExplorePermissionsWidgetFilterProxyModel::ExplorePermissionsWidgetFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

ExplorePermissionsWidgetFilterProxyModel::~ExplorePermissionsWidgetFilterProxyModel() = default;

bool ExplorePermissionsWidgetFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mFilterString.isEmpty()) {
        return true;
    }
    auto match = [&](int role) {
        const QModelIndex modelIndex = sourceModel()->index(source_row, role, source_parent);
        return modelIndex.data(0).toString().contains(mFilterString, Qt::CaseInsensitive);
    };
    if (match(PermissionsModel::RolesStr) || match(PermissionsModel::Identifier)) {
        return true;
    }
    return false;
}

void ExplorePermissionsWidgetFilterProxyModel::setFilterString(const QString &string)
{
    mFilterString = string;
    invalidate();
}
