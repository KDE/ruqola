/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionswidgetfilterproxymodel.h"

ExplorePermissionsWidgetFilterProxyModel::ExplorePermissionsWidgetFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

ExplorePermissionsWidgetFilterProxyModel::~ExplorePermissionsWidgetFilterProxyModel() = default;
