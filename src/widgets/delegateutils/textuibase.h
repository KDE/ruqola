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
class QListView;

class TextUiBase
{
public:
    explicit TextUiBase(TextSelectionImpl *textSelectionImpl, QListView *view);
    virtual ~TextUiBase();

protected:
    virtual void removeMessageCache(const QString &messageId);
    virtual void setCacheMaxEntries(int maxEntries);
    virtual void clearCache();
    mutable LRUCache<QString, std::unique_ptr<QTextDocument>> mDocumentCache;
    TextSelectionImpl *const mTextSelectionImpl;
    QListView *const mListView;
};
