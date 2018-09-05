/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "facebookauthjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
FacebookAuthJob::FacebookAuthJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

FacebookAuthJob::~FacebookAuthJob()
{
}

bool FacebookAuthJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start facebook login job";
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
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);

    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    addLoggerInfo("FacebookAuthJob started " + baPostData);
    connect(reply, &QNetworkReply::finished, this, &FacebookAuthJob::slotFacebookauthDone);

    return false;
}

void FacebookAuthJob::slotFacebookauthDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("status")].toString() == QStringLiteral("success") && replyObject.contains(QLatin1String("data"))) {
            QJsonObject data = replyObject[QStringLiteral("data")].toObject();

            if (data.contains(QLatin1String("authToken")) && data.contains(QLatin1String("userId"))) {
                const QString authToken = data[QStringLiteral("authToken")].toString();
                const QString userId = data[QStringLiteral("userId")].toString();
                Q_EMIT facebookDone(authToken, userId);
            }
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Error during login" << data;
        }
    }
    deleteLater();
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
    loginMap.insert(QStringLiteral("serviceName"), QStringLiteral("facebook"));
    loginMap.insert(QStringLiteral("accessToken"), mAccessToken);
    loginMap.insert(QStringLiteral("secret"), mSecret);
    loginMap.insert(QStringLiteral("expiresIn"), mExpireTokenInSeconds);
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest FacebookAuthJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

bool FacebookAuthJob::requireHttpAuthentication() const
{
    return false;
}
