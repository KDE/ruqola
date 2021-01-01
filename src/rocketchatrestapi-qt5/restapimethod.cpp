/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
using namespace RocketChatRestApi;
RestApiMethod::RestApiMethod()
{
}

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
