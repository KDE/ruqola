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
#include "delegate/messagelistdelegate.h"
#include "dialogs/reportmessagedialog.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QKeyEvent>
#include <QMenu>
#include <QScrollBar>
#include <QIcon>
#include <QPointer>
#include <QClipboard>
#include <QApplication>

MessageListView::MessageListView(Mode mode, QWidget *parent)
    : QListView(parent)
    , mMode(mode)
{
    auto *delegate = new MessageListDelegate(this);
    delegate->setRocketChatAccount(Ruqola::self()->rocketChatAccount());
    delegate->setShowThreadContext(mMode != Mode::ThreadEditing);
    setItemDelegate(delegate);

    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // nicer in case of huge messages
    setWordWrap(true); // so that the delegate sizeHint is called again when the width changes

    // only the lineedit takes focus
    setFocusPolicy(Qt::NoFocus);

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageListView::slotVerticalScrollbarChanged);
}

MessageListView::~MessageListView()
{
}

void MessageListView::slotVerticalScrollbarChanged(int value)
{
    if (value == 0) {
        Q_EMIT loadHistoryRequested();
        //Perhaps finding a better method.
        verticalScrollBar()->setValue(1); //If we are at 0 we can't continue to load history
    }
}

void MessageListView::setRoomId(const QString &roomID)
{
    mRoomId = roomID;
}

void MessageListView::setChannelSelected(const QString &roomId)
{
    MessageModel *oldModel = qobject_cast<MessageModel *>(model());
    if (oldModel) {
        oldModel->deactivate();
    }
    Ruqola::self()->rocketChatAccount()->switchingToRoom(roomId);
    MessageModel *model = Ruqola::self()->rocketChatAccount()->messageModelForRoom(roomId);
    setModel(model);
    model->activate();
    mRoomId = roomId;
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

void MessageListView::handleKeyPressEvent(QKeyEvent *ev)
{
    const int key = ev->key();
    if (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_PageDown || key == Qt::Key_PageUp) {
        // QListView/QAIV PageUp/PageDown moves the current item, first inside visible bounds
        // before it triggers scrolling around. Let's just let the scrollarea handle it,
        // since we don't show the current item.
        QAbstractScrollArea::keyPressEvent(ev);
        ev->accept();
    } else if (key == Qt::Key_Home && ev->modifiers() & Qt::ControlModifier) {
        scrollToTop();
        ev->accept();
    } else if (key == Qt::Key_End && ev->modifiers() & Qt::ControlModifier) {
        scrollToBottom();
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
    const bool isSystemMessage = index.data(MessageModel::MessageType).value<Message::MessageType>() == Message::System;
    if (isSystemMessage) {
        return;
    }

    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QMenu menu(this);
    QAction *copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy"), &menu);
    connect(copyAction, &QAction::triggered, this, [=]() {
        slotCopyText(index);
    });
    //TODO fix me we can't pinned message when we are not owner
    QAction *setPinnedMessage = nullptr;
    if (rcAccount->allowMessagePinningEnabled()) {
        const bool isPinned = index.data(MessageModel::Pinned).toBool();
        setPinnedMessage = new QAction(QIcon::fromTheme(QStringLiteral("pin")), isPinned ? i18n("Unpin Message") : i18n("Pin Message"), &menu);
        connect(setPinnedMessage, &QAction::triggered, this, [this, isPinned, index]() {
            slotSetPinnedMessage(index, isPinned);
        });
    }
    QAction *setAsFavoriteAction = nullptr;
    if (rcAccount->allowMessageStarringEnabled()) {
        const bool isStarred = index.data(MessageModel::Starred).toBool();
        setAsFavoriteAction = new QAction(QIcon::fromTheme(QStringLiteral("favorite")), isStarred ? i18n("Remove as Favorite") : i18n("Set as Favorite"), &menu);
        connect(setAsFavoriteAction, &QAction::triggered, this, [this, isStarred, index]() {
            slotSetAsFavorite(index, isStarred);
        });
    }
    QAction *deleteAction = nullptr;
    if (index.data(MessageModel::CanDeleteMessage).toBool()) {
        deleteAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-delete")), i18n("Delete"), &menu);
        connect(deleteAction, &QAction::triggered, this, [=]() {
            slotDeleteMessage(index);
        });
    }

    if (mMode == Mode::Editing) {
        QAction *startDiscussion = new QAction(i18n("Start a Discussion"), &menu);
        connect(startDiscussion, &QAction::triggered, this, [=]() {
            slotStartDiscussion(index);
        });
        menu.addAction(startDiscussion);

        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
        }

        if (index.data(MessageModel::CanEditMessage).toBool()) {
            QAction *editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")), i18n("Edit"), &menu);
            connect(editAction, &QAction::triggered, this, [=]() {
                slotEditMessage(index);
            });
            menu.addAction(editAction);
        }
        menu.addAction(copyAction);

        if (deleteAction) {
            createSeparator(menu);
            menu.addAction(deleteAction);
        }
        if (rcAccount->autoTranslateEnabled()) {
            createSeparator(menu);
            const bool isTranslated = index.data(MessageModel::ShowTranslatedMessage).toBool();
            QAction *translateAction = new QAction(isTranslated ? i18n("Show Original Message") : i18n("Translate Message"), &menu);
            connect(translateAction, &QAction::triggered, this, [=](bool checked) {
                slotTranslateMessage(index, checked);
            });
            menu.addAction(translateAction);
        }
    } else if (mMode == Mode::ThreadEditing) {
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
        }
        if (deleteAction) {
            createSeparator(menu);
            menu.addAction(deleteAction);
        }
    } else {
#if 0
        if (setPinnedMessage) {
            menu.addAction(setPinnedMessage);
        }
        if (setAsFavoriteAction) {
            menu.addAction(setAsFavoriteAction);
        }
#endif
        menu.addAction(copyAction);
        createSeparator(menu);
        QAction *goToMessageAction = new QAction(i18n("Go to Message"), &menu); //Add icon
        connect(goToMessageAction, &QAction::triggered, this, [=]() {
            slotGoToMessage(index);
        });
        menu.addAction(goToMessageAction);
    }

    createSeparator(menu);
    QAction *reportMessageAction = new QAction(QIcon::fromTheme(QStringLiteral("messagebox_warning")), i18n("Report Message"), &menu);
    connect(reportMessageAction, &QAction::triggered, this, [=]() {
        slotReportMessage(index);
    });
    menu.addAction(reportMessageAction);

    if (!menu.actions().isEmpty()) {
        menu.exec(event->globalPos());
    }
}

void MessageListView::createSeparator(QMenu &menu)
{
    if (!menu.isEmpty()) {
        auto *separator = new QAction(&menu);
        separator->setSeparator(true);
        menu.addAction(separator);
    }
}

void MessageListView::slotTranslateMessage(const QModelIndex &index, bool checked)
{
    qDebug() << "No implemented yet";
    //TODO
}

void MessageListView::slotGoToMessage(const QModelIndex &index)
{
    qDebug() << "No implemented yet";
    //TODO
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
        rcAccount->deleteMessage(messageId, mRoomId);
    }
}

void MessageListView::slotReportMessage(const QModelIndex &index)
{
    QPointer<ReportMessageDialog> dlg = new ReportMessageDialog(this);
    const QString message = index.data(MessageModel::OriginalMessage).toString();
    dlg->setPreviewMessage(message);
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
}

void MessageListView::slotStartDiscussion(const QModelIndex &index)
{
    const QString message = index.data(MessageModel::OriginalMessage).toString();
    const QString messageId = index.data(MessageModel::MessageId).toString();
    Q_EMIT createNewDiscussion(messageId, message);
}

void MessageListView::slotCopyText(const QModelIndex &index)
{
    const QString messageStr = index.data(MessageModel::OriginalMessage).toString();
    QClipboard *clip = QApplication::clipboard();
    clip->setText(messageStr, QClipboard::Clipboard);
    clip->setText(messageStr, QClipboard::Selection);
}

MessageListView::Mode MessageListView::mode() const
{
    return mMode;
}
