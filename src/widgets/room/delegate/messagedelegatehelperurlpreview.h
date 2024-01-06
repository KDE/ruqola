/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "messagedelegatehelperbase.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperUrlPreview : public MessageDelegateHelperBase
{
public:
    explicit MessageDelegateHelperUrlPreview(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperUrlPreview() override;
};
