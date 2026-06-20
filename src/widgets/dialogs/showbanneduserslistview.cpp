/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbanneduserslistview.h"
#include <KLocalizedString>
#include <QMenu>
#include <qevent.h>

ShowBannedUsersListView::ShowBannedUsersListView(QWidget *parent)
    : QListView(parent)
{
}

ShowBannedUsersListView::~ShowBannedUsersListView() = default;

void ShowBannedUsersListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }
    // TODO
    QMenu menu(this);
    menu.exec(event->globalPos());
}

#include "moc_showbanneduserslistview.cpp"
