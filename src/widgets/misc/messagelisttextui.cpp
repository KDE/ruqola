/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelisttextui.h"

MessageListTextUi::MessageListTextUi(TextSelectionImpl *textSelectionImpl, QListView *view)
    : TextUiBase(textSelectionImpl, view)
{
}

MessageListTextUi::~MessageListTextUi() = default;
