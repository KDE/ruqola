/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionlistview.h"
#include "discussion/listdiscussiondelegate.h"
#include "model/discussionsmodel.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QMenu>
#include <QMouseEvent>

#include <config-ruqola.h>

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
    connect(this, &DiscussionListView::needToClearSizeHintCache, mListDiscussionDelegate, &ListDiscussionDelegate::clearSizeHintCache);
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
        if (mListDiscussionDelegate->hasSelection()) {
            addTextPlugins(&menu, mListDiscussionDelegate->selectedText());
        }
#if HAVE_TEXT_TO_SPEECH
        menu.addSeparator();
        auto speakAction = menu.addAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")), i18n("Speak Text"));
        connect(speakAction, &QAction::triggered, this, [index, this]() {
            slotTextToSpeech(index);
        });
#endif
        menu.addSeparator();
        menu.addAction(i18n("Select All"), this, [this, index]() {
            slotSelectAll(index);
        });
    }
    if (!menu.isEmpty()) {
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void DiscussionListView::slotTextToSpeech(const QModelIndex &index)
{
    QString message = mListDiscussionDelegate->selectedText();
    if (message.isEmpty()) {
        message = index.data(DiscussionsModel::LastMessage).toString();
    }
    if (!message.isEmpty()) {
        Q_EMIT textToSpeech(message);
    }
}

void DiscussionListView::slotSelectAll(const QModelIndex &index)
{
    mListDiscussionDelegate->selectAll(listViewOptions(), index);
}

#include "moc_discussionlistview.cpp"
