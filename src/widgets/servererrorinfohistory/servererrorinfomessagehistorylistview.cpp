/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorylistview.h"

ServerErrorInfoMessageHistoryListView::ServerErrorInfoMessageHistoryListView(QWidget *parent)
    : MessageListViewBase(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    //    connect(mListNotificationsDelegate, &NotificationHistoryDelegate::updateView, this, [this](const QModelIndex &index) {
    //        update(index);
    //    });
    connect(this, &QListView::customContextMenuRequested, this, &ServerErrorInfoMessageHistoryListView::slotCustomContextMenuRequested);
}

ServerErrorInfoMessageHistoryListView::~ServerErrorInfoMessageHistoryListView() = default;

void ServerErrorInfoMessageHistoryListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (model()->rowCount() > 0) {
        // TODO
    }
}
