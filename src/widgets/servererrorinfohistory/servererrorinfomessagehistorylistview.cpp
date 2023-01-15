/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorylistview.h"
#include "servererrorinfohistorydelegate.h"

#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>

ServerErrorInfoMessageHistoryListView::ServerErrorInfoMessageHistoryListView(QWidget *parent)
    : MessageListViewBase(parent)
    , mListServerErrorInfosDelegate(new ServerErrorInfoHistoryDelegate(this, this))
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    mListServerErrorInfosDelegate->setObjectName(QStringLiteral("mListServerErrorInfosDelegate"));
    setItemDelegate(mListServerErrorInfosDelegate);

    connect(mListServerErrorInfosDelegate, &ServerErrorInfoHistoryDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    connect(this, &QListView::customContextMenuRequested, this, &ServerErrorInfoMessageHistoryListView::slotCustomContextMenuRequested);
}

ServerErrorInfoMessageHistoryListView::~ServerErrorInfoMessageHistoryListView() = default;

void ServerErrorInfoMessageHistoryListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (model()->rowCount() > 0) {
        // TODO
    }
}

bool ServerErrorInfoMessageHistoryListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListServerErrorInfosDelegate->maybeStartDrag(event, option, index);
}

bool ServerErrorInfoMessageHistoryListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mListServerErrorInfosDelegate->mouseEvent(event, option, index);
}

void ServerErrorInfoMessageHistoryListView::clearCache()
{
    mListServerErrorInfosDelegate->clearCache();
}

void ServerErrorInfoMessageHistoryListView::slotSelectAll(const QModelIndex &index)
{
    mListServerErrorInfosDelegate->selectAll(listViewOptions(), index);
}

const QString &ServerErrorInfoMessageHistoryListView::searchText() const
{
    return mListServerErrorInfosDelegate->searchText();
}

void ServerErrorInfoMessageHistoryListView::setSearchText(const QString &newSearchText)
{
    mListServerErrorInfosDelegate->setSearchText(newSearchText);
}

QString ServerErrorInfoMessageHistoryListView::selectedText() const
{
    return mListServerErrorInfosDelegate->selectedText();
}

#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
void ServerErrorInfoMessageHistoryListView::slotTextToSpeech(const QModelIndex &index)
{
    QString messageText = selectedText();
    if (messageText.isEmpty()) {
        if (!index.isValid()) {
            return;
        }
        // TODO messageText = index.data(NotificationHistoryModel::MessageStr).toString();
    }
    Q_EMIT textToSpeech(messageText);
}
#endif

void ServerErrorInfoMessageHistoryListView::slotClearList()
{
    clearCache();
    // NotificationHistoryManager::self()->notificationHistoryModel()->clear();
}

void ServerErrorInfoMessageHistoryListView::copyMessageToClipboard(const QModelIndex &index)
{
    QString message = selectedText();
    if (message.isEmpty()) {
        if (!index.isValid()) {
            return;
        }
        // TODO message = index.data(NotificationHistoryModel::MessageStr).toString();
    }

    QClipboard *clip = QApplication::clipboard();
    clip->setText(message, QClipboard::Clipboard);
    clip->setText(message, QClipboard::Selection);
}
