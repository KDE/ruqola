/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionstreeview.h"
#include <QHeaderView>

PermissionsTreeView::PermissionsTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setRootIsDecorated(false);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    header()->setSectionsClickable(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

PermissionsTreeView::~PermissionsTreeView() = default;

#include "moc_permissionstreeview.cpp"
