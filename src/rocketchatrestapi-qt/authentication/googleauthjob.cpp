/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googleauthjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GoogleAuthJob::GoogleAuthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GoogleAuthJob::~GoogleAuthJob() = default;

bool GoogleAuthJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mAccessToken.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Access Token is empty";
        return false;
    }
    if (mIdToken.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "IdToken is empty";
        return false;
    }
    if (mExpireTokenInSeconds <= 0) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Expire token is not defined";
        return false;
    }
    return true;
}

bool GoogleAuthJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitPostRequest(json());
    return true;
}

void GoogleAuthJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("status")].toString() == QLatin1String("success") && replyObject.contains(QLatin1String("data"))) {
        const QJsonObject dataObject = replyObject[QLatin1String("data")].toObject();

        if (dataObject.contains(QLatin1String("authToken")) && dataObject.contains(QLatin1String("userId"))) {
            const QString authToken = dataObject[QLatin1String("authToken")].toString();
            const QString userId = dataObject[QLatin1String("userId")].toString();
            Q_EMIT googleauthDone(authToken, userId);
        }
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Error during login" + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GoogleAuthJob::idToken() const
{
    return mIdToken;
}

void GoogleAuthJob::setIdToken(const QString &idToken)
{
    mIdToken = idToken;
}

int GoogleAuthJob::expireTokenInSeconds() const
{
    return mExpireTokenInSeconds;
}

void GoogleAuthJob::setExpireTokenInSeconds(int expireTokenInSeconds)
{
    mExpireTokenInSeconds = expireTokenInSeconds;
}

QString GoogleAuthJob::accessToken() const
{
    return mAccessToken;
}

void GoogleAuthJob::setAccessToken(const QString &accessToken)
{
    mAccessToken = accessToken;
}

QJsonDocument GoogleAuthJob::json() const
{
    QVariantMap loginMap;
    loginMap.insert(QStringLiteral("serviceName"), QStringLiteral("google"));
    loginMap.insert(QStringLiteral("accessToken"), mAccessToken);
    loginMap.insert(QStringLiteral("idToken"), mIdToken);
    loginMap.insert(QStringLiteral("expiresIn"), mExpireTokenInSeconds);
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest GoogleAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

bool GoogleAuthJob::requireHttpAuthentication() const
{
    return false;
}

#include "moc_googleauthjob.cpp"
