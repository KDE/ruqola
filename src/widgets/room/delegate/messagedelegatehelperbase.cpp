/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperbase.h"

#include "rocketchataccount.h"
#include <QListView>

MessageDelegateHelperBase::MessageDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : mListView(view)
    , mSelectionImpl(textSelectionImpl)
    , mRocketChatAccount(account)
{
}

MessageDelegateHelperBase::~MessageDelegateHelperBase() = default;
