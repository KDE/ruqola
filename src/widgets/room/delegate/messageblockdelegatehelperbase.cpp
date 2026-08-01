/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageblockdelegatehelperbase.h"

#include <QAbstractItemView>

MessageBlockDelegateHelperBase::MessageBlockDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageBlockDelegateHelperBase::~MessageBlockDelegateHelperBase() = default;

bool MessageBlockDelegateHelperBase::handleHelpEvent([[maybe_unused]] QHelpEvent *helpEvent,
                                                     [[maybe_unused]] QRect messageRect,
                                                     [[maybe_unused]] const Block &block,
                                                     [[maybe_unused]] const QStyleOptionViewItem &option)
{
    return false;
}

#include "moc_messageblockdelegatehelperbase.cpp"
