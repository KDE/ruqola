/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "lrucache.h"
#include <QString>
#include <QTextDocument>
#include <memory>

class TextSelectionImpl;
class QAbstractItemView;

class TextUiBase
{
public:
    explicit TextUiBase(TextSelectionImpl *textSelectionImpl, QAbstractItemView *view);
    virtual ~TextUiBase();

protected:
    virtual void removeMessageCache(const QByteArray &messageId);
    virtual void setCacheMaxEntries(int maxEntries);
    virtual void clearCache();
    mutable LRUCache<QByteArray, std::unique_ptr<QTextDocument>> mDocumentCache;
    TextSelectionImpl *const mTextSelectionImpl;
    QAbstractItemView *const mListView;
};
