/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperurlpreview.h"

MessageDelegateHelperUrlPreview::MessageDelegateHelperUrlPreview(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperUrlPreview::~MessageDelegateHelperUrlPreview() = default;
