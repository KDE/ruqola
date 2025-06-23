/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "githubauthenticationsettings.h"
using namespace Qt::Literals::StringLiterals;

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
    return QUrl(u"https://github.com/login/oauth/authorize"_s);
}

QUrl GitHubAuthenticationSettings::accessTokenUrl()
{
    return QUrl(u"https://github.com/login/oauth/access_token"_s);
}
