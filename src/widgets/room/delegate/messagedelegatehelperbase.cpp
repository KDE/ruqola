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

void MessageDelegateHelperBase::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

void MessageDelegateHelperBase::updateView(const QModelIndex &index)
{
    // qDebug() << " void MessageDelegateHelperText::updateView(const QModelIndex &index)" << index;
    mListView->update(index);
}

void MessageDelegateHelperBase::removeMessageCache(const QString &messageId)
{
    mDocumentCache.remove(messageId);
}
