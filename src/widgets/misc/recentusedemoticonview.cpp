/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "recentusedemoticonview.h"
#include <KLocalizedString>
#include <QContextMenuEvent>
#include <QMenu>

RecentUsedEmoticonView::RecentUsedEmoticonView(QWidget *parent)
    : EmoticonListViewBase(parent)
{
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

RecentUsedEmoticonView::~RecentUsedEmoticonView() = default;

void RecentUsedEmoticonView::contextMenuEvent(QContextMenuEvent *event)
{
    if (model()->rowCount() > 0) {
        QMenu menu(this);
        auto clearRecent = new QAction(i18n("Clear Recents"), &menu);
        connect(clearRecent, &QAction::triggered, this, &RecentUsedEmoticonView::clearAll);
        menu.addAction(clearRecent);
        menu.exec(event->globalPos());
    }
}
