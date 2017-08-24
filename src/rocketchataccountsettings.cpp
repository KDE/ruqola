/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "rocketchataccountsettings.h"
#include "ruqola_debug.h"

#include <QSettings>
#include <QStandardPaths>

RocketChatAccountSettings::RocketChatAccountSettings()
{
}

QString RocketChatAccountSettings::userId() const
{
    return mUserId;
}

void RocketChatAccountSettings::setUserId(const QString &userId)
{
    mUserId = userId;
}

QString RocketChatAccountSettings::authToken() const
{
    return mAuthToken;
}

void RocketChatAccountSettings::setAuthToken(const QString &authToken)
{
    if (mAuthToken != authToken) {
        qCDebug(RUQOLA_LOG) << "Setting token to" << authToken;
        QSettings s;
        mAuthToken = authToken;
        s.setValue(QStringLiteral("authToken"), authToken);
    }
}

void RocketChatAccountSettings::logout()
{
    QSettings s;
    s.setValue(QStringLiteral("authToken"), QString());
    mAuthToken.clear();
    mUserId.clear();
    mPassword.clear();
}

void RocketChatAccountSettings::saveSettings()
{
    //TODO
}

void RocketChatAccountSettings::loadSettings()
{
    QSettings s;
    //TODO add specific group name.
    mServerUrl = s.value(QStringLiteral("serverURL"), QStringLiteral("demo.rocket.chat")).toString();
    mUserName = s.value(QStringLiteral("username")).toString();
    mUserId = s.value(QStringLiteral("userID")).toString();
    mAuthToken = s.value(QStringLiteral("authToken")).toString();
}

QString RocketChatAccountSettings::password() const
{
    return mPassword;
}

void RocketChatAccountSettings::setPassword(const QString &password)
{
    mPassword = password;
}

QString RocketChatAccountSettings::userName() const
{
    return mUserName;
}

void RocketChatAccountSettings::setUserName(const QString &userName)
{
    mUserName = userName;
}

QString RocketChatAccountSettings::accountName() const
{
    return mAccountName;
}

void RocketChatAccountSettings::setAccountName(const QString &accountName)
{
    mAccountName = accountName;
}

QString RocketChatAccountSettings::serverUrl() const
{
    return mServerUrl;
}

void RocketChatAccountSettings::setServerUrl(const QString &serverUrl)
{
    mServerUrl = serverUrl;
}

QString RocketChatAccountSettings::cacheBasePath()
{
    if (mServerUrl.isEmpty()) {
        return QString();
    }
    if (mCachePath.isEmpty()) {
        mCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1Char('/') + mServerUrl;
    }
    return mCachePath;
}

