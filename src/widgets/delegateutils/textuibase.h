/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "lrucache.h"
#include <QString>
#include <QTextDocument>
#include <memory>

class TextSelectionImpl;
class QTextDocument;
class QListView;

class TextUiBase
{
public:
    explicit TextUiBase(TextSelectionImpl *textSelectionImpl, QListView *view);
    virtual ~TextUiBase();

protected:
    void removeMessageCache(const QString &messageId);
    void setCacheMaxEntries(int maxEntries);
    void clearCache();
    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QString, QSize> mSizeHintCache;
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>> mDocumentCache;
    TextSelectionImpl *const mTextSelectionImpl;
    QListView *const mListView;
};
