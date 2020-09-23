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

#include "twitterauthjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TwitterAuthJob::TwitterAuthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TwitterAuthJob::~TwitterAuthJob()
{
}

bool TwitterAuthJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mAccessToken.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Access Token is empty";
        return false;
    }
    if (mSecret.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Secret is empty";
        return false;
    }
    if (mAppId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Appid is empty";
        return false;
    }
    if (mAppSecret.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "App secret is empty";
        return false;
    }
    if (mExpireTokenInSeconds <= 0) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Expire token is not defined";
        return false;
    }
    return true;
}

bool TwitterAuthJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TwitterAuthJob::slotTwitterauthDone);
    return true;
}

void TwitterAuthJob::slotTwitterauthDone()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("status")].toString() == QLatin1String("success") && replyObject.contains(QLatin1String("data"))) {
            const QJsonObject dataObject = replyObject[QStringLiteral("data")].toObject();

            if (dataObject.contains(QLatin1String("authToken")) && dataObject.contains(QLatin1String("userId"))) {
                const QString authToken = dataObject[QStringLiteral("authToken")].toString();
                const QString userId = dataObject[QStringLiteral("userId")].toString();
                Q_EMIT twitterDone(authToken, userId);
            }
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning("Error during login" + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString TwitterAuthJob::appId() const
{
    return mAppId;
}

void TwitterAuthJob::setAppId(const QString &appId)
{
    mAppId = appId;
}

QString TwitterAuthJob::appSecret() const
{
    return mAppSecret;
}

void TwitterAuthJob::setAppSecret(const QString &appSecret)
{
    mAppSecret = appSecret;
}

int TwitterAuthJob::expireTokenInSeconds() const
{
    return mExpireTokenInSeconds;
}

void TwitterAuthJob::setExpireTokenInSeconds(int expireTokenInSeconds)
{
    mExpireTokenInSeconds = expireTokenInSeconds;
}

QString TwitterAuthJob::secret() const
{
    return mSecret;
}

void TwitterAuthJob::setSecret(const QString &secret)
{
    mSecret = secret;
}

QString TwitterAuthJob::accessToken() const
{
    return mAccessToken;
}

void TwitterAuthJob::setAccessToken(const QString &accessToken)
{
    mAccessToken = accessToken;
}

QJsonDocument TwitterAuthJob::json() const
{
    QVariantMap loginMap;
    loginMap.insert(QStringLiteral("serviceName"), QStringLiteral("twitter"));
    loginMap.insert(QStringLiteral("accessToken"), mAccessToken);
    loginMap.insert(QStringLiteral("accessTokenSecret"), mSecret);
    loginMap.insert(QStringLiteral("appSecret"), mAppSecret);
    loginMap.insert(QStringLiteral("appId"), mAppId);
    loginMap.insert(QStringLiteral("expiresIn"), mExpireTokenInSeconds);
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest TwitterAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

bool TwitterAuthJob::requireHttpAuthentication() const
{
    return false;
}
