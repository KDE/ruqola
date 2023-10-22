/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textuibase.h"

class MessageListTextUi : public TextUiBase
{
public:
    explicit MessageListTextUi(TextSelectionImpl *textSelectionImpl, QListView *view);
    ~MessageListTextUi() override;

    void clearSizeHintCache();

protected:
    void removeMessageCache(const QString &messageId) override;
    void setCacheMaxEntries(int maxEntries) override;
    void clearCache() override;
    // Cache SizeHint value
    // We need to clear it when we resize widget.
    mutable LRUCache<QString, QSize> mSizeHintCache;
};
