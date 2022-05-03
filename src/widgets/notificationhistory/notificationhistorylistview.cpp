/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorylistview.h"
#include "notificationhistorydelegate.h"

#include <QMouseEvent>

NotificationHistoryListView::NotificationHistoryListView(QWidget *parent)
    : MessageListViewBase(parent)
    , mListNotificationsDelegate(new NotificationHistoryDelegate(this, this))
{
    mListNotificationsDelegate->setObjectName(QStringLiteral("listNotificationsDelegate"));
    setItemDelegate(mListNotificationsDelegate);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mListNotificationsDelegate, &NotificationHistoryDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
}

NotificationHistoryListView::~NotificationHistoryListView()
{
}

bool NotificationHistoryListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListNotificationsDelegate->maybeStartDrag(event, option, index);
}

bool NotificationHistoryListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListNotificationsDelegate->mouseEvent(event, option, index);
}

void NotificationHistoryListView::clearCache()
{
    mListNotificationsDelegate->clearCache();
}

void NotificationHistoryListView::slotSelectAll(const QModelIndex &index)
{
    mListNotificationsDelegate->selectAll(listViewOptions(), index);
}
