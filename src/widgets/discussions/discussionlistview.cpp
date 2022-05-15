/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionlistview.h"
#include "discussion/listdiscussiondelegate.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include <KLocalizedString>

#include <QMenu>
#include <QMouseEvent>

DiscussionListView::DiscussionListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mListDiscussionDelegate(new ListDiscussionDelegate(this, account, this))
    , mRocketChatAccount(account)
{
    setItemDelegate(mListDiscussionDelegate);
    connect(mListDiscussionDelegate, &ListDiscussionDelegate::openDiscussion, this, &DiscussionListView::slotOpenDiscussion);
    connect(mListDiscussionDelegate, &ListDiscussionDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QListView::customContextMenuRequested, this, &DiscussionListView::slotCustomContextMenuRequested);
}

DiscussionListView::~DiscussionListView() = default;

void DiscussionListView::setSearchText(const QString &str)
{
    mListDiscussionDelegate->setSearchText(str);
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

void DiscussionListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = indexAt(pos);
    if (index.isValid()) {
        menu.addAction(i18n("Select All"), this, [this, index]() {
            slotSelectAll(index);
        });
    }
    if (!menu.isEmpty()) {
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void DiscussionListView::slotSelectAll(const QModelIndex &index)
{
    mListDiscussionDelegate->selectAll(listViewOptions(), index);
}
