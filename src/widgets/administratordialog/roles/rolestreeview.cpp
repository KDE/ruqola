/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolestreeview.h"
#include <QHeaderView>

RolesTreeView::RolesTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setRootIsDecorated(false);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    header()->setSectionsClickable(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

RolesTreeView::~RolesTreeView() = default;

#include "moc_rolestreeview.cpp"
