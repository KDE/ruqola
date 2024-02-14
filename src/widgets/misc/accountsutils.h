/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QIcon>
class RocketChatAccount;
namespace AccountsUtils
{
struct AccountInfo {
    QString name;
    QIcon icon;
};
[[nodiscard]] QIcon iconFromAccount(RocketChatAccount *account);
};
Q_DECLARE_TYPEINFO(AccountsUtils::AccountInfo, Q_RELOCATABLE_TYPE);
