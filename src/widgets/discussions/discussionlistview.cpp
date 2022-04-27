/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionlistview.h"
#include "discussion/listdiscussiondelegate.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <QMouseEvent>

DiscussionListView::DiscussionListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mListDiscussionDelegate(new ListDiscussionDelegate(account, this))
    , mRocketChatAccount(account)
{
    setItemDelegate(mListDiscussionDelegate);
    connect(mListDiscussionDelegate, &ListDiscussionDelegate::openDiscussion, this, &DiscussionListView::slotOpenDiscussion);
}

DiscussionListView::~DiscussionListView()
{
}

bool DiscussionListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListDiscussionDelegate->maybeStartDrag(event, option, index);
}

bool DiscussionListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListDiscussionDelegate->mouseEvent(event, option, index);
}

void DiscussionListView::slotOpenDiscussion(const QString &roomDiscussionId)
{
    if (mRocketChatAccount) {
        mRocketChatAccount->ddp()->openRoom(roomDiscussionId);
    }
}
