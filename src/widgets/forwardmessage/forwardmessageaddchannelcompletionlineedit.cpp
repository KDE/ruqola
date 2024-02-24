/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessageaddchannelcompletionlineedit.h"
#include "common/completionlistview.h"
#include "misc/joinedchannelcompletiondelegate.h"
#include "rocketchataccount.h"

using namespace std::chrono_literals;

ForwardMessageAddChannelCompletionLineEdit::ForwardMessageAddChannelCompletionLineEdit(RocketChatAccount *account, QWidget *parent)
    : JoinedChannelCompletionLineEditBase(account, parent)
{
    connect(this, &ForwardMessageAddChannelCompletionLineEdit::joinedChannelFound, this, &ForwardMessageAddChannelCompletionLineEdit::slotJoinedChannelFound);
}

ForwardMessageAddChannelCompletionLineEdit::~ForwardMessageAddChannelCompletionLineEdit() = default;

void ForwardMessageAddChannelCompletionLineEdit::slotJoinedChannelFound(
    const ForwardMessageAddChannelCompletionLineEdit::JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &info)
{
    disconnect(this, &QLineEdit::textChanged, this, &ForwardMessageAddChannelCompletionLineEdit::slotSearchTextEdited);
    Q_EMIT fowardToChannel(std::move(info));
    clear();
    connect(this, &QLineEdit::textChanged, this, &ForwardMessageAddChannelCompletionLineEdit::slotSearchTextEdited);
}

#include "moc_forwardmessageaddchannelcompletionlineedit.cpp"
