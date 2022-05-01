/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverconfiginfo.h"
#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"

ServerConfigInfo::ServerConfigInfo(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
}

ServerConfigInfo::~ServerConfigInfo() = default;

QString ServerConfigInfo::serverVersionStr() const
{
    if (mAccount) {
        return mAccount->ruqolaServerConfig()->serverVersionStr();
    }
    return {};
}

QString ServerConfigInfo::accountName() const
{
    if (mAccount) {
        return mAccount->settings()->accountName();
    }
    return {};
}

QString ServerConfigInfo::userName() const
{
    if (mAccount) {
        return mAccount->settings()->userName();
    }
    return {};
}

QString ServerConfigInfo::serverUrl() const
{
    if (mAccount) {
        return mAccount->settings()->serverUrl();
    }
    return {};
}

QString ServerConfigInfo::logoUrl() const
{
    if (mAccount) {
        const QString logoUrl = mAccount->ruqolaServerConfig()->logoUrl();
        if (logoUrl.isEmpty()) {
            return {};
        }
        return mAccount->settings()->serverUrl() + QLatin1Char('/') + logoUrl;
    }
    return {};
}

QString ServerConfigInfo::faviconUrl() const
{
    if (mAccount) {
        const QString faviconUrl = mAccount->ruqolaServerConfig()->faviconUrl();
        if (faviconUrl.isEmpty()) {
            return {};
        }
        return mAccount->settings()->serverUrl() + QLatin1Char('/') + faviconUrl;
    }
    return {};
}
