/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "messagedelegatehelperbase.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageBlockDelegateHelperBase : public MessageDelegateHelperBase
{
public:
    explicit MessageBlockDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    virtual ~MessageBlockDelegateHelperBase();
};
