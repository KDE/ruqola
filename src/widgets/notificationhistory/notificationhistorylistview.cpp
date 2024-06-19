/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorylistview.h"
#include "model/notificationhistorymodel.h"
#include "notificationhistorydelegate.h"
#include "notificationhistorymanager.h"

#include <KLocalizedString>

#include <QMenu>
#include <QMouseEvent>

#include "config-ruqola.h"

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
    connect(this, &NotificationHistoryListView::needToClearSizeHintCache, mListNotificationsDelegate, &NotificationHistoryDelegate::clearSizeHintCache);
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
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-clear-history")), i18nc("@action", "Clear"), this, &NotificationHistoryListView::slotClearList);
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
            menu.addSeparator();
            menu.addAction(i18nc("@action", "Go to Message"), this, [this, index]() {
                Q_EMIT showMessage(index);
            });
            menu.addSeparator();
            auto copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18nc("@action", "Copy Message"), &menu);
            copyAction->setShortcut(QKeySequence::Copy);
            connect(copyAction, &QAction::triggered, this, [this, index]() {
                copyMessageToClipboard(index);
            });
            menu.addAction(copyAction);
            if (mListNotificationsDelegate->hasSelection()) {
                addTextPlugins(&menu, mListNotificationsDelegate->selectedText());
            }
#if HAVE_TEXT_TO_SPEECH
            menu.addSeparator();
            auto speakAction = menu.addAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")), i18nc("@action", "Speak Text"));
            connect(speakAction, &QAction::triggered, this, [this, index]() {
                slotTextToSpeech(index);
            });
#endif
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(QStringLiteral("edit-select-all")), i18nc("@action", "Select All"), this, [this, index]() {
                slotSelectAll(index);
            });
        }
        menu.exec(viewport()->mapToGlobal(pos));
    }
}

void NotificationHistoryListView::slotTextToSpeech(const QModelIndex &index)
{
    const QString messageText = selectedText(index);
    if (!messageText.isEmpty()) {
        Q_EMIT textToSpeech(messageText);
    }
}

QString NotificationHistoryListView::selectedText(const QModelIndex &index)
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

void NotificationHistoryListView::slotClearList()
{
    clearCache();
    NotificationHistoryManager::self()->notificationHistoryModel()->clear();
}

#include "moc_notificationhistorylistview.cpp"
