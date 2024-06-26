/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textuibase.h"

TextUiBase::TextUiBase(TextSelectionImpl *textSelectionImpl, QAbstractItemView *view)
    : mTextSelectionImpl(textSelectionImpl)
    , mListView(view)
{
}

TextUiBase::~TextUiBase() = default;

void TextUiBase::removeMessageCache(const QByteArray &messageId)
{
    mDocumentCache.remove(messageId);
}

void TextUiBase::setCacheMaxEntries(int maxEntries)
{
    mDocumentCache.setMaxEntries(maxEntries);
}

void TextUiBase::clearCache()
{
    mDocumentCache.clear();
}
