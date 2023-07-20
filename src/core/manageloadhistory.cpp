/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "manageloadhistory.h"

ManageLoadHistory::ManageLoadHistory(RocketChatAccount *account, QObject *parent)
    : QObject{parent}
    , mAccount(account)
{
}

ManageLoadHistory::~ManageLoadHistory() = default;
