/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "restapimethod.h"
using namespace Qt::Literals::StringLiterals;

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

QUrl RestApiMethod::generateUrl(RestApiUtil::RestApiUrlType type, RestApiUtil::RestApiUrlExtensionType restApiUrlExtensionType, const QString &urlExtension)
{
    if (mServerUrl.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Server url is empty";
        return {};
    }
    const QString restUrl = RestApiUtil::restUrl(type);
    QString urlStr = RestApiUtil::adaptUrl(mServerUrl) + RestApiUtil::restApiUrlExtensionType(restApiUrlExtensionType) + restUrl;
    if (!urlExtension.isEmpty()) {
        if (!restUrl.isEmpty()) {
            urlStr += u'/';
        }
        urlStr += urlExtension;
    }
    return QUrl(urlStr);
}
