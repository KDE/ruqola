/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"
#include "libruqolawidgets_private_export.h"
class QListView;
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperBase : public QObject, public DocumentFactoryInterface
{
public:
    explicit MessageDelegateHelperBase(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    virtual ~MessageDelegateHelperBase();

protected:
    QListView *const mListView;
    TextSelectionImpl *const mSelectionImpl;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
