/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

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
#if 0
    if (!mDocumentCache.remove(messageId)) {
        qCWarning(RUQOLA_CACHE_LOG) << " impossible to find identifier " << messageId;
    }
#endif
}

void TextUiBase::setCacheMaxEntries(int maxEntries)
{
    mDocumentCache.setMaxEntries(maxEntries);
}

void TextUiBase::clearCache()
{
    mDocumentCache.clear();
}
