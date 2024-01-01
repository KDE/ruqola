/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationsettings.h"

using namespace GitHubAuthenticationSettings;

QString GitHubAuthenticationSettings::clientId()
{
    return {};
}

QString GitHubAuthenticationSettings::clientSecret()
{
    return {};
}

QUrl GitHubAuthenticationSettings::authorizeUrl()
{
    return QUrl(QStringLiteral("https://github.com/login/oauth/authorize"));
}

QUrl GitHubAuthenticationSettings::accessTokenUrl()
{
    return QUrl(QStringLiteral("https://github.com/login/oauth/access_token"));
}
