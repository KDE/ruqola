/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messages/message.h"
class EmojiManager;
class LIBRUQOLACORE_EXPORT ModerationMessage : public Message
{
public:
    explicit ModerationMessage(EmojiManager *emojiManager = nullptr);
    ~ModerationMessage();
};
