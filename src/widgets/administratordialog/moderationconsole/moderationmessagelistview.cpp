/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessagelistview.h"
#include "model/notificationhistorymodel.h"
#include "moderationmessagedelegate.h"

#include <KLocalizedString>

#include <QMenu>
#include <QMouseEvent>

#include <config-ruqola.h>

ModerationMessageListView::ModerationMessageListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mListNotificationsDelegate(new ModerationMessageDelegate(this, account, this))
{
    mListNotificationsDelegate->setObjectName(QStringLiteral("listNotificationsDelegate"));
    setItemDelegate(mListNotificationsDelegate);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(mListNotificationsDelegate, &ModerationMessageDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    connect(this, &QListView::customContextMenuRequested, this, &ModerationMessageListView::slotCustomContextMenuRequested);
}

ModerationMessageListView::~ModerationMessageListView() = default;

bool ModerationMessageListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListNotificationsDelegate->maybeStartDrag(event, option, index);
}

bool ModerationMessageListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListNotificationsDelegate->mouseEvent(event, option, index);
}

void ModerationMessageListView::clearCache()
{
    mListNotificationsDelegate->clearCache();
}

void ModerationMessageListView::slotSelectAll(const QModelIndex &index)
{
    mListNotificationsDelegate->selectAll(listViewOptions(), index);
}

const QString &ModerationMessageListView::searchText() const
{
    return mListNotificationsDelegate->searchText();
}

void ModerationMessageListView::setSearchText(const QString &newSearchText)
{
    mListNotificationsDelegate->setSearchText(newSearchText);
}

QString ModerationMessageListView::selectedText() const
{
    return mListNotificationsDelegate->selectedText();
}

void ModerationMessageListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (model()->rowCount() > 0) {
        QMenu menu(this);
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
            // TODO add show moderation infos!

            auto showReportInfo = new QAction(i18n("View Reports"), &menu); // Add icon
            connect(showReportInfo, &QAction::triggered, this, [=]() {
                // TODO
                // const QString messageId = message->messageId();
                // const QString messageDateTimeUtc = index.data(MessagesModel::DateTimeUtc).toString();
                // Q_EMIT goToMessageRequested(messageId, messageDateTimeUtc);
            });

            menu.addSeparator();
            menu.addAction(i18n("Go to Message"), this, [this, index]() {
                Q_EMIT showMessage(index);
            });
            menu.addSeparator();
            auto copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy Message"), &menu);
            copyAction->setShortcut(QKeySequence::Copy);
            connect(copyAction, &QAction::triggered, this, [=]() {
                copyMessageToClipboard(index);
            });
            menu.addAction(copyAction);
            if (mListNotificationsDelegate->hasSelection()) {
                addTextPlugins(&menu, mListNotificationsDelegate->selectedText());
            }
#if HAVE_TEXT_TO_SPEECH
            menu.addSeparator();
            auto speakAction = menu.addAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")), i18n("Speak Text"));
            connect(speakAction, &QAction::triggered, this, [=]() {
                slotTextToSpeech(index);
            });
#endif
            menu.addSeparator();
            menu.addAction(i18n("Select All"), this, [this, index]() {
                slotSelectAll(index);
            });
        }
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void ModerationMessageListView::slotTextToSpeech(const QModelIndex &index)
{
    const QString messageText = selectedText(index);
    if (!messageText.isEmpty()) {
        Q_EMIT textToSpeech(messageText);
    }
}

QString ModerationMessageListView::selectedText(const QModelIndex &index)
{
    QString messageText = selectedText();
    if (messageText.isEmpty()) {
        if (!index.isValid()) {
            return {};
        }
        messageText = index.data(NotificationHistoryModel::MessageStr).toString();
    }
    return messageText;
}

#include "moc_moderationmessagelistview.cpp"
