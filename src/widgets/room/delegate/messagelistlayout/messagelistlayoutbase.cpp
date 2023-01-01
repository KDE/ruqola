/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistlayoutbase.h"
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
