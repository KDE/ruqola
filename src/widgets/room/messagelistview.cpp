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
#include <QDebug>
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
    //TODO temporary
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
    scrollToBottom();
}

void MessageListView::checkIfAtBottom()
{
    auto *vbar = verticalScrollBar();
    mAtBottom = vbar->value() == vbar->maximum();
    Q_EMIT modelChanged();
}

void MessageListView::maybeScrollToBottom()
{
    if (mAtBottom) {
        scrollToBottom();
    }
    Q_EMIT modelChanged();
}
