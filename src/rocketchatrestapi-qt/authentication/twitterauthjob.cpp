/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twitterauthjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TwitterAuthJob::TwitterAuthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TwitterAuthJob::~TwitterAuthJob() = default;

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
    submitPostRequest(json());
    return true;
}

void TwitterAuthJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("status")].toString() == QLatin1String("success") && replyObject.contains(QLatin1String("data"))) {
        const QJsonObject dataObject = replyObject[QLatin1String("data")].toObject();

        if (dataObject.contains(QLatin1String("authToken")) && dataObject.contains(QLatin1String("userId"))) {
            const QString authToken = dataObject[QLatin1String("authToken")].toString();
            const QString userId = dataObject[QLatin1String("userId")].toString();
            Q_EMIT twitterDone(authToken, userId);
        }
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Error during login" + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_twitterauthjob.cpp"
