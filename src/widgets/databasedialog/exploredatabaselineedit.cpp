/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exploredatabaselineedit.h"

ExploreDatabaseLineEdit::ExploreDatabaseLineEdit(RocketChatAccount *account, QWidget *parent)
    : JoinedChannelCompletionLineEditBase(account, parent)
{
    connect(this, &ExploreDatabaseLineEdit::joinedChannelFound, this, &ExploreDatabaseLineEdit::slotJoinedChannelFound);
}

ExploreDatabaseLineEdit::~ExploreDatabaseLineEdit() = default;

void ExploreDatabaseLineEdit::slotJoinedChannelFound(const JoinedChannelCompletionLineEditBase::JoinedChannelCompletionInfo &info)
{
    disconnect(this, &QLineEdit::textChanged, this, &ExploreDatabaseLineEdit::slotSearchTextEdited);
    setText(info.name);
    connect(this, &QLineEdit::textChanged, this, &ExploreDatabaseLineEdit::slotSearchTextEdited);
}

#include "moc_exploredatabaselineedit.cpp"
