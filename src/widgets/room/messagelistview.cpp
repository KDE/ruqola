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

#include <KLocalizedString>

#include <QDebug>
#include <QKeyEvent>
#include <QMenu>
#include <QScrollBar>

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
    if (!index.isValid())
        return;
    QMenu menu(this);
    if (index.data(MessageModel::CanEditMessage).toBool()) {
        QAction *editAction = new QAction(i18n("Edit"), &menu);
        connect(editAction, &QAction::triggered, this, [=]() { slotEditMessage(index); });
        menu.addAction(editAction);
    }
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
