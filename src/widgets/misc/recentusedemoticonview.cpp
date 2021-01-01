/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "recentusedemoticonview.h"
#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>

RecentUsedEmoticonView::RecentUsedEmoticonView(QWidget *parent)
    : QListView(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

RecentUsedEmoticonView::~RecentUsedEmoticonView()
{
}

void RecentUsedEmoticonView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *clearRecent = new QAction(i18n("Clear Recents"), &menu);
    connect(clearRecent, &QAction::triggered, this, &RecentUsedEmoticonView::clearAll);
    menu.addAction(clearRecent);
    menu.exec(event->globalPos());
}
