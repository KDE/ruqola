/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsutils.h"
#include "rocketchataccount.h"

QIcon AccountsUtils::iconFromAccount(RocketChatAccount *account)
{
    QIcon icon;
    const QString iconFaviconUrl{account->attachmentUrlFromLocalCache(account->ruqolaServerConfig()->faviconUrl().url).toLocalFile()};
    if (!iconFaviconUrl.isEmpty()) {
        const QIcon iconFavicon{iconFaviconUrl};
        if (!iconFavicon.isNull()) {
            icon = std::move(iconFavicon);
        }
    }
    return icon;
}
