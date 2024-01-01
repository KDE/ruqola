/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
using namespace RocketChatRestApi;
RestApiMethod::RestApiMethod() = default;

QString RestApiMethod::serverUrl() const
{
    return mServerUrl;
}

void RestApiMethod::setServerUrl(const QString &serverUrl)
{
    mServerUrl = serverUrl;
}

QUrl RestApiMethod::generateUrl(RestApiUtil::RestApiUrlType type, const QString &urlExtension, bool useV1Extension)
{
    if (mServerUrl.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Server url is empty";
        return {};
    }
    QString urlStr = RestApiUtil::adaptUrl(mServerUrl) + (useV1Extension ? RestApiUtil::apiUri() : QStringLiteral("/api/")) + RestApiUtil::restUrl(type);
    if (!urlExtension.isEmpty()) {
        urlStr += QLatin1Char('/') + urlExtension;
    }
    return QUrl(urlStr);
}
