/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "textuibase.h"

TextUiBase::TextUiBase(TextSelectionImpl *textSelectionImpl, QListView *view)
    : mTextSelectionImpl(textSelectionImpl)
    , mListView(view)
{
}

TextUiBase::~TextUiBase() = default;

void TextUiBase::removeMessageCache(const QString &messageId)
{
    mDocumentCache.remove(messageId);
    mSizeHintCache.remove(messageId);
}

void TextUiBase::setCacheMaxEntries(int maxEntries)
{
    mDocumentCache.setMaxEntries(maxEntries);
    mSizeHintCache.setMaxEntries(maxEntries);
}

void TextUiBase::clearSizeHintCache()
{
    mSizeHintCache.clear();
}

void TextUiBase::clearCache()
{
    mDocumentCache.clear();
    mSizeHintCache.clear();
}
