/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidgetfilterproxymodel.h"
#include "model/permissionsmodel.h"

ExplorePermissionsWidgetFilterProxyModel::ExplorePermissionsWidgetFilterProxyModel(QObject *parent)
    : SortFilterProxyModelBase{parent}
{
}

ExplorePermissionsWidgetFilterProxyModel::~ExplorePermissionsWidgetFilterProxyModel() = default;

bool ExplorePermissionsWidgetFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mFilterString.isEmpty()) {
        return true;
    }
    auto match = [&](int column) {
        const QModelIndex modelIndex = sourceModel()->index(source_row, column, source_parent);
        return contains(modelIndex.data(Qt::DisplayRole).toString());
    };
    return match(PermissionsModel::RolesStr) || match(PermissionsModel::Identifier);
}

#include "moc_explorepermissionswidgetfilterproxymodel.cpp"
