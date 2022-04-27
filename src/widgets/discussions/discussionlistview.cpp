/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionlistview.h"

DiscussionListView::DiscussionListView(QWidget *parent)
    : MessageListViewBase(parent)
{
}

DiscussionListView::~DiscussionListView()
{
}

bool DiscussionListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // TODO
    return MessageListViewBase::maybeStartDrag(event, option, index);
}

bool DiscussionListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // TODO
    return MessageListViewBase::maybeStartDrag(event, option, index);
}
