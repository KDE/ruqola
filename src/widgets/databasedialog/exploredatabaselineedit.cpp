/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

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
    mChannelId = info.channelId;
    connect(this, &QLineEdit::textChanged, this, &ExploreDatabaseLineEdit::slotSearchTextEdited);
}

QByteArray ExploreDatabaseLineEdit::channelId() const
{
    return mChannelId;
}

void ExploreDatabaseLineEdit::setChannelId(const QByteArray &newChannelId)
{
    mChannelId = newChannelId;
}

#include "moc_exploredatabaselineedit.cpp"
