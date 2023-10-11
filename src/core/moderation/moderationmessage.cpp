/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessage.h"

ModerationMessage::ModerationMessage(EmojiManager *emojiManager)
    : Message(emojiManager)
{
}

ModerationMessage::~ModerationMessage() = default;
