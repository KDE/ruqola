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

void MessageListTextUi::removeMessageCache(const QString &messageId)
{
    mSizeHintCache.remove(messageId);
    TextUiBase::removeMessageCache(messageId);
}

void MessageListTextUi::setCacheMaxEntries(int maxEntries)
{
    mSizeHintCache.setMaxEntries(maxEntries);
    TextUiBase::setCacheMaxEntries(maxEntries);
}

void MessageListTextUi::clearCache()
{
    mSizeHintCache.clear();
    TextUiBase::clearCache();
}
