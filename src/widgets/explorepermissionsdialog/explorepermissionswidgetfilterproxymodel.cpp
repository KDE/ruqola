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
    return QSortFilterProxyModel::filterAcceptsColumn(source_row, source_parent);
#if 0
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString identifier = sourceIndex.data(PermissionsModel::Identifier).toString();
    return (mTypeGroup == typegroup) && QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
#endif
}
