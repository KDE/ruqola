/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "facebookauthjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
FacebookAuthJob::FacebookAuthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FacebookAuthJob::~FacebookAuthJob() = default;

bool FacebookAuthJob::canStart() const
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
    if (mExpireTokenInSeconds <= 0) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Expire token is not defined";
        return false;
    }
    return true;
}

bool FacebookAuthJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    submitPostRequest(json());
    return true;
}

void FacebookAuthJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["status"_L1].toString() == "success"_L1 && replyObject.contains("data"_L1)) {
        const QJsonObject dataObject = replyObject["data"_L1].toObject();

        if (dataObject.contains("authToken"_L1) && dataObject.contains("userId"_L1)) {
            const QString authToken = dataObject["authToken"_L1].toString();
            const QString userId = dataObject["userId"_L1].toString();
            Q_EMIT facebookDone(authToken, userId);
        }
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Error during login" + replyJson.toJson(QJsonDocument::Indented));
    }
}

int FacebookAuthJob::expireTokenInSeconds() const
{
    return mExpireTokenInSeconds;
}

void FacebookAuthJob::setExpireTokenInSeconds(int expireTokenInSeconds)
{
    mExpireTokenInSeconds = expireTokenInSeconds;
}

QString FacebookAuthJob::secret() const
{
    return mSecret;
}

void FacebookAuthJob::setSecret(const QString &secret)
{
    mSecret = secret;
}

QString FacebookAuthJob::accessToken() const
{
    return mAccessToken;
}

void FacebookAuthJob::setAccessToken(const QString &accessToken)
{
    mAccessToken = accessToken;
}

QJsonDocument FacebookAuthJob::json() const
{
    QVariantMap loginMap;
    loginMap.insert(u"serviceName"_s, u"facebook"_s);
    loginMap.insert(u"accessToken"_s, mAccessToken);
    loginMap.insert(u"secret"_s, mSecret);
    loginMap.insert(u"expiresIn"_s, mExpireTokenInSeconds);
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest FacebookAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, u"application/json"_s);
    return request;
}

bool FacebookAuthJob::requireHttpAuthentication() const
{
    return false;
}

#include "moc_facebookauthjob.cpp"
