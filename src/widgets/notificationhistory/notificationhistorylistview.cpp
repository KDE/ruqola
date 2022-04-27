/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorylistview.h"
#include "notificationhistorydelegate.h"

#include <QMouseEvent>

NotificationHistoryListView::NotificationHistoryListView(QWidget *parent)
    : MessageListViewBase(parent)
    , mListNotificationsDelegate(new NotificationHistoryDelegate(this))
{
    mListNotificationsDelegate->setObjectName(QStringLiteral("listNotificationsDelegate"));
    setItemDelegate(mListNotificationsDelegate);
}

NotificationHistoryListView::~NotificationHistoryListView()
{
}

bool NotificationHistoryListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // TODO
    return MessageListViewBase::maybeStartDrag(event, option, index);
}

bool NotificationHistoryListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListNotificationsDelegate->mouseEvent(event, option, index);
}

void NotificationHistoryListView::clearCache()
{
    mListNotificationsDelegate->clearCache();
}
