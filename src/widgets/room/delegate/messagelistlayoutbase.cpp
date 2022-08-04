/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistlayoutbase.h"

MessageListLayoutBase::MessageListLayoutBase() = default;

MessageListLayoutBase::Layout MessageListLayoutBase::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return {};
}

RocketChatAccount *MessageListLayoutBase::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void MessageListLayoutBase::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}
