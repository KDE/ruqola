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

#include "roomhelper.h"
#include "rocketchataccount.h"
#include "room/messagelinewidget.h"
#include "room/messagelistview.h"

RoomHelper::RoomHelper(QObject *parent)
    : QObject(parent)
{
}

RoomHelper::~RoomHelper()
{
}

QString RoomHelper::messageIdBeingEdited() const
{
    return mMessageIdBeingEdited;
}

void RoomHelper::setMessageIdBeingEdited(const QString &messageIdBeingEdited)
{
    mMessageIdBeingEdited = messageIdBeingEdited;
}

MessageListView *RoomHelper::messageListView() const
{
    return mMessageListView;
}

void RoomHelper::setMessageListView(MessageListView *messageListView)
{
    mMessageListView = messageListView;
}

QString RoomHelper::roomId() const
{
    return mRoomId;
}

void RoomHelper::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

RocketChatAccount *RoomHelper::currentRocketChatAccount() const
{
    return mCurrentRocketChatAccount;
}

void RoomHelper::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mCurrentRocketChatAccount = currentRocketChatAccount;
}

void RoomHelper::slotEditMessage(const QString &messageId, const QString &text)
{
    mMessageIdBeingEdited = messageId;
    mMessageLineWidget->setMode(MessageLineWidget::EditingMode::EditMessage);
    mMessageLineWidget->setText(text);
    mMessageLineWidget->setFocus();
}

void RoomHelper::slotSendMessage(const QString &msg)
{
    if (mMessageIdBeingEdited.isEmpty()) {
        mCurrentRocketChatAccount->sendMessage(mRoomId, msg);
    } else {
        mCurrentRocketChatAccount->updateMessage(mRoomId, mMessageIdBeingEdited, msg);
        mMessageIdBeingEdited.clear();
    }
    mMessageLineWidget->setMode(MessageLineWidget::EditingMode::NewMessage);
}

MessageLineWidget *RoomHelper::messageLineWidget() const
{
    return mMessageLineWidget;
}

void RoomHelper::setMessageLineWidget(MessageLineWidget *messageLineWidget)
{
    mMessageLineWidget = messageLineWidget;
}
