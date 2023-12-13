/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistlayoutbase.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messagelistdelegate.h"

MessageListLayoutBase::MessageListLayoutBase(MessageListDelegate *delegate)
    : mDelegate(delegate)
{
}

MessageListLayoutBase::~MessageListLayoutBase() = default;

RocketChatAccount *MessageListLayoutBase::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void MessageListLayoutBase::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

bool MessageListLayoutBase::sameSenderAsPreviousMessage(const QModelIndex &index, const Message *message) const
{
    if (index.row() < 1) {
        return false;
    }

    const auto previousIndex = index.siblingAtRow(index.row() - 1);
    const auto previousMessage = previousIndex.data(MessagesModel::MessagePointer).value<Message *>();
    Q_ASSERT(previousMessage);

    const int diffDate = mRocketChatAccount ? mRocketChatAccount->ruqolaServerConfig()->messageGroupingPeriod() * 1000 : 0;
    if ((message->userId() == previousMessage->userId()) && (message->threadMessageId() == previousMessage->threadMessageId())
        && (message->timeStamp() <= (previousMessage->timeStamp() + diffDate)))
        return true;
    return false;
}

QString MessageListLayoutBase::senderText(const Message *message) const
{
    if (mRocketChatAccount) {
        return QLatin1Char('@') + (mRocketChatAccount->useRealName() && !message->name().isEmpty() ? message->name() : message->username());
    } else {
        return QLatin1Char('@') + message->username();
    }
}
