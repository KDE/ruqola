/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "twitterauthjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
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
    if (replyObject["status"_L1].toString() == "success"_L1 && replyObject.contains("data"_L1)) {
        const QJsonObject dataObject = replyObject["data"_L1].toObject();

        if (dataObject.contains("authToken"_L1) && dataObject.contains("userId"_L1)) {
            const QString authToken = dataObject["authToken"_L1].toString();
            const QString userId = dataObject["userId"_L1].toString();
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
    loginMap.insert(u"serviceName"_s, u"twitter"_s);
    loginMap.insert(u"accessToken"_s, mAccessToken);
    loginMap.insert(u"accessTokenSecret"_s, mSecret);
    loginMap.insert(u"appSecret"_s, mAppSecret);
    loginMap.insert(u"appId"_s, mAppId);
    loginMap.insert(u"expiresIn"_s, mExpireTokenInSeconds);
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest TwitterAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, u"application/json"_s);
    return request;
}

bool TwitterAuthJob::requireHttpAuthentication() const
{
    return false;
}

#include "moc_twitterauthjob.cpp"
