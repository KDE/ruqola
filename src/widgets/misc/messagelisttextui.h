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
};
