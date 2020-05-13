/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "rocketchataccount.h"
#include "serverconfiginfo.h"
#include "ruqolaserverconfig.h"

ServerConfigInfo::ServerConfigInfo(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mAccount(account)
{
}

ServerConfigInfo::~ServerConfigInfo()
{
}

QString ServerConfigInfo::serverVersionStr() const
{
    if (mAccount) {
        return mAccount->ruqolaServerConfig()->serverVersionStr();
    }
    return {};
}

QString ServerConfigInfo::serverName() const
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
