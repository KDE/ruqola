/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelisttextui.h"

MessageListTextUi::MessageListTextUi(TextSelectionImpl *textSelectionImpl, QAbstractItemView *view)
    : TextUiBase(textSelectionImpl, view)
{
}

void MessageListTextUi::clearSizeHintCache()
{
    mSizeHintCache.clear();
}

MessageListTextUi::~MessageListTextUi() = default;

void MessageListTextUi::removeMessageCache(const QByteArray &messageId)
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
