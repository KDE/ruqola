/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "googleauthjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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
    if (replyObject["status"_L1].toString() == "success"_L1 && replyObject.contains("data"_L1)) {
        const QJsonObject dataObject = replyObject["data"_L1].toObject();

        if (dataObject.contains("authToken"_L1) && dataObject.contains("userId"_L1)) {
            const QString authToken = dataObject["authToken"_L1].toString();
            const QString userId = dataObject["userId"_L1].toString();
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
    loginMap.insert(u"serviceName"_s, u"google"_s);
    loginMap.insert(u"accessToken"_s, mAccessToken);
    loginMap.insert(u"idToken"_s, mIdToken);
    loginMap.insert(u"expiresIn"_s, mExpireTokenInSeconds);
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest GoogleAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, u"application/json"_s);
    return request;
}

bool GoogleAuthJob::requireHttpAuthentication() const
{
    return false;
}

#include "moc_googleauthjob.cpp"
