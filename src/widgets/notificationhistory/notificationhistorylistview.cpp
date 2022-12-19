/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorylistview.h"
#include "model/notificationhistorymodel.h"
#include "notificationhistorydelegate.h"
#include "notificationhistorymanager.h"

#include <KLocalizedString>

#include <QApplication>
#include <QClipboard>
#include <QMenu>
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
    connect(this, &QListView::customContextMenuRequested, this, &NotificationHistoryListView::slotCustomContextMenuRequested);
}

NotificationHistoryListView::~NotificationHistoryListView() = default;

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

const QString &NotificationHistoryListView::searchText() const
{
    return mListNotificationsDelegate->searchText();
}

void NotificationHistoryListView::setSearchText(const QString &newSearchText)
{
    mListNotificationsDelegate->setSearchText(newSearchText);
}

QString NotificationHistoryListView::selectedText() const
{
    return mListNotificationsDelegate->selectedText();
}

void NotificationHistoryListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (model()->rowCount() > 0) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-clear-history")), i18n("Clear"), this, &NotificationHistoryListView::slotClearList);
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
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
#if HAVE_TEXT_TO_SPEECH_SUPPORT
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

#if HAVE_TEXT_TO_SPEECH_SUPPORT
void NotificationHistoryListView::slotTextToSpeech(const QModelIndex &index)
{
    QString messageText = selectedText();
    if (messageText.isEmpty()) {
        if (!index.isValid()) {
            return;
        }
        messageText = index.data(NotificationHistoryModel::MessageStr).toString();
    }
    Q_EMIT textToSpeech(messageText);
}
#endif

void NotificationHistoryListView::slotClearList()
{
    clearCache();
    NotificationHistoryManager::self()->notificationHistoryModel()->clear();
}

void NotificationHistoryListView::copyMessageToClipboard(const QModelIndex &index)
{
    QString message = selectedText();
    if (message.isEmpty()) {
        if (!index.isValid()) {
            return;
        }
        message = index.data(NotificationHistoryModel::MessageStr).toString();
    }

    QClipboard *clip = QApplication::clipboard();
    clip->setText(message, QClipboard::Clipboard);
    clip->setText(message, QClipboard::Selection);
}
