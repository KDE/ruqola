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

protected:
    void removeMessageCache(const QString &messageId) override;
    void setCacheMaxEntries(int maxEntries) override;
    void clearCache() override;
    mutable LRUCache<QString, QSize> mSizeHintCache;
};
