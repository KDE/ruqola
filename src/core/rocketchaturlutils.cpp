/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchaturlutils.h"
using namespace Qt::Literals::StringLiterals;

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
        info.accountName = u"importaccount"_s;
        // Add authentication support
        if (!parseInfo.token.isEmpty() && !parseInfo.userId.isEmpty()) {
            info.token = parseInfo.token;
            info.userId = parseInfo.userId;
            info.authMethodType = AuthenticationManager::AuthMethodType::PersonalAccessToken;
        }
        Ruqola::self()->accountManager()->addAccount(std::move(info));

        auto notification = new KNotification(u"Account-Added"_s, KNotification::CloseOnTimeout);
        notification->setTitle(i18n("Account Added"));
        notification->setText(i18n("A new account was added."));
        notification->sendEvent();
        return true;
    }
    case ParseRocketChatUrlUtils::UrlType::Invite: {
        AccountManager::AccountManagerInfo info;
        info.serverUrl = parseUrl.parsingInfo().serverHost;
        info.enabled = true;
        info.accountName = u"inviteaccount"_s;
        info.authMethodType = AuthenticationManager::AuthMethodType::Password;
        info.inviteToken = parseUrl.parsingInfo().path.remove(u"invite/"_s);
        Ruqola::self()->accountManager()->addInvitedAccount(std::move(info));
        return true;
    }
    case ParseRocketChatUrlUtils::UrlType::ConferenceCall:
        qDebug() << " ParseRocketChatUrlUtils::UrlType ConferenceCall Not implement yet ";
        return true;
    case ParseRocketChatUrlUtils::UrlType::Unknown:
        break;
    }
    return false;
}
