/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorylistview.h"

NotificationHistoryListView::NotificationHistoryListView(QWidget *parent)
    : MessageListViewBase(parent)
{
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
    // TODO
    return MessageListViewBase::maybeStartDrag(event, option, index);
}
