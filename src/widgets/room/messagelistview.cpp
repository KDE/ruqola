/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messagelistview.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "messagelistdelegate.h"
#include "dialogs/reportmessagedialog.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QKeyEvent>
#include <QMenu>
#include <QScrollBar>
#include <QIcon>
#include <QPointer>

MessageListView::MessageListView(QWidget *parent)
    : QListView(parent)
{
    auto *delegate = new MessageListDelegate(this);
    delegate->setRocketChatAccount(Ruqola::self()->rocketChatAccount());
    setItemDelegate(delegate);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // nicer in case of huge messages
    setWordWrap(true); // so that the delegate sizeHint is called again when the width changes
}

MessageListView::~MessageListView()
{
}

void MessageListView::setChannelSelected(const QString &roomId)
{
    Ruqola::self()->rocketChatAccount()->switchingToRoom(roomId);
    setModel(Ruqola::self()->rocketChatAccount()->messageModelForRoom(roomId));
    mRoomID = roomId;
}

void MessageListView::setModel(QAbstractItemModel *newModel)
{
    QAbstractItemModel *oldModel = model();
    if (oldModel) {
        disconnect(oldModel, nullptr, this, nullptr);
    }
    QListView::setModel(newModel);
    connect(newModel, &QAbstractItemModel::rowsAboutToBeInserted, this, &MessageListView::checkIfAtBottom);
    connect(newModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &MessageListView::checkIfAtBottom);
    connect(newModel, &QAbstractItemModel::modelAboutToBeReset, this, &MessageListView::checkIfAtBottom);
    connect(newModel, &QAbstractItemModel::rowsInserted, this, &MessageListView::maybeScrollToBottom);
    connect(newModel, &QAbstractItemModel::rowsRemoved, this, &MessageListView::maybeScrollToBottom);
    connect(newModel, &QAbstractItemModel::modelReset, this, &MessageListView::maybeScrollToBottom);

    connect(newModel, &QAbstractItemModel::rowsInserted, this, &MessageListView::modelChanged);
    connect(newModel, &QAbstractItemModel::rowsRemoved, this, &MessageListView::modelChanged);
    connect(newModel, &QAbstractItemModel::modelReset, this, &MessageListView::modelChanged);

    scrollToBottom();
}

void MessageListView::resizeEvent(QResizeEvent *ev)
{
    QListView::resizeEvent(ev);

    // Fix not being really at bottom when the view gets reduced by the header widget becoming taller
    checkIfAtBottom();
    maybeScrollToBottom(); // this forces a layout in QAIV, which then changes the vbar max value
}

void MessageListView::keyPressEvent(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_PageDown || key == Qt::Key_PageUp
        || key == Qt::Key_Home || key == Qt::Key_End) {
        QListView::keyPressEvent(ev);
    } else {
        // If the user starts typing a message focus the lineedit and send the event there
        emit keyPressed(ev);
        ev->accept();
    }
}

void MessageListView::checkIfAtBottom()
{
    auto *vbar = verticalScrollBar();
    mAtBottom = vbar->value() == vbar->maximum();
}

void MessageListView::maybeScrollToBottom()
{
    if (mAtBottom) {
        scrollToBottom();
    }
}

void MessageListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    if (!index.isValid()) {
        return;
    }
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QMenu menu(this);

    const bool isPinned = index.data(MessageModel::Pinned).toBool();
    QAction *setPinnedMessage = new QAction(QIcon::fromTheme(QStringLiteral("pin")), isPinned ? i18n("Unpin Message") : i18n("Pin Message"), &menu);
    connect(setPinnedMessage, &QAction::triggered, this, [this, isPinned, index]() {
        slotSetPinnedMessage(index, isPinned);
    });
    menu.addAction(setPinnedMessage);

    const bool isStarred = index.data(MessageModel::Starred).toBool();
    QAction *setAsFavoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")), isStarred ? i18n("Remove as Favorite") : i18n("Set as Favorite"), &menu);
    connect(setAsFavoriteAction, &QAction::triggered, this, [this, isStarred, index]() {
        slotSetAsFavorite(index, isStarred);
    });
    menu.addAction(setAsFavoriteAction);

    if (rcAccount->allowEditingMessages() && index.data(MessageModel::CanEditMessage).toBool()) {
        QAction *editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Edit"), &menu);
        connect(editAction, &QAction::triggered, this, [=]() {
            slotEditMessage(index);
        });
        menu.addAction(editAction);
    }
    if (rcAccount->allowMessageDeletingEnabled() && index.data(MessageModel::UserId).toString() == rcAccount->userID()) {
        if (!menu.isEmpty()) {
            QAction *separator = new QAction(&menu);
            separator->setSeparator(true);
            menu.addAction(separator);
        }
        QAction *deleteAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-delete")), i18n("Delete"), &menu);
        connect(deleteAction, &QAction::triggered, this, [=]() {
            slotDeleteMessage(index);
        });
        menu.addAction(deleteAction);
    }
    if (!menu.isEmpty()) {
        QAction *separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
    }
    QAction *reportMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("messagebox_warning")), i18n("Report Message"), &menu);
    connect(reportMessageAction, &QAction::triggered, this, [=]() {
        slotReportMessage(index);
    });
    menu.addAction(reportMessageAction);
    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

void MessageListView::slotEditMessage(const QModelIndex &index)
{
    const QString text = index.data(MessageModel::OriginalMessage).toString();
    const QString messageId = index.data(MessageModel::MessageId).toString();
    Q_EMIT editMessageRequested(messageId, text);
}

void MessageListView::slotDeleteMessage(const QModelIndex &index)
{
    if (KMessageBox::Yes == KMessageBox::questionYesNo(this, i18n("Do you want to delete this message?"), i18n("Delete Message"))) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        const QString messageId = index.data(MessageModel::MessageId).toString();
        rcAccount->deleteMessage(messageId, mRoomID);
    }
}

void MessageListView::slotReportMessage(const QModelIndex &index)
{
    QPointer<ReportMessageDialog> dlg = new ReportMessageDialog(this);
    if (dlg->exec()) {
        auto *rcAccount = Ruqola::self()->rocketChatAccount();
        const QString messageId = index.data(MessageModel::MessageId).toString();
        rcAccount->reportMessage(messageId, dlg->message());
    }
    delete dlg;
}

void MessageListView::slotSetAsFavorite(const QModelIndex &index, bool isStarred)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString messageId = index.data(MessageModel::MessageId).toString();
    rcAccount->starMessage(messageId, !isStarred);
}

void MessageListView::slotSetPinnedMessage(const QModelIndex &index, bool isPinned)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QString messageId = index.data(MessageModel::MessageId).toString();
    rcAccount->pinMessage(messageId, !isPinned);
    //TODO fix pinMessage it seems that it doesn't work
}
