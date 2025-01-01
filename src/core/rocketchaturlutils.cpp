/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchaturlutils.h"
#include "accountmanager.h"
#include "parserocketchaturlutils.h"
#include "ruqola.h"

#include <KLocalizedString>
#include <KNotification>

bool RocketChatUrlUtils::parseUrl(const QString &link)
{
    ParseRocketChatUrlUtils parseUrl;
    switch (parseUrl.parseUrl(link)) {
    case ParseRocketChatUrlUtils::UrlType::Room:
    case ParseRocketChatUrlUtils::UrlType::Message: {
        if (Ruqola::self()->accountManager()->showMessage(std::move(parseUrl.parsingInfo()))) {
            return true;
        }
        break;
    }
    case ParseRocketChatUrlUtils::UrlType::Server: {
        const ParseRocketChatUrlUtils::ParsingInfo parseInfo = parseUrl.parsingInfo();
        AccountManager::AccountManagerInfo info;
        info.serverUrl = parseInfo.serverHost;
        info.enabled = true;
        info.accountName = QStringLiteral("importaccount");
        // Add authentication support
        if (!parseInfo.token.isEmpty() && !parseInfo.userId.isEmpty()) {
            info.token = parseInfo.token;
            info.userId = parseInfo.userId;
            info.authMethodType = AuthenticationManager::AuthMethodType::PersonalAccessToken;
        }
        Ruqola::self()->accountManager()->addAccount(std::move(info));

        auto notification = new KNotification(QStringLiteral("Account-Added"), KNotification::CloseOnTimeout);
        notification->setTitle(i18n("Account Added"));
        notification->setText(i18n("A new account was added."));
        notification->sendEvent();
        return true;
    }
    case ParseRocketChatUrlUtils::UrlType::Invite: {
        AccountManager::AccountManagerInfo info;
        info.serverUrl = parseUrl.parsingInfo().serverHost;
        // TODO
        break;
    }
    case ParseRocketChatUrlUtils::UrlType::ConferenceCall:
        qDebug() << " ParseRocketChatUrlUtils::UrlType ConferenceCall Not implement yet ";
        return true;
    case ParseRocketChatUrlUtils::UrlType::Unknown:
        break;
    }
    return false;
}
