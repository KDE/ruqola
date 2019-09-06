/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "loginjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LoginJob::LoginJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LoginJob::~LoginJob()
{
}

bool LoginJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start login job";
        return false;
    }
    if (mUserName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "UserName is empty";
        return false;
    }
    if (mPassword.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Password is empty";
        return false;
    }
    return true;
}

bool LoginJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);

    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    addLoggerInfo("LoginJob started " + baPostData);
    connect(reply, &QNetworkReply::finished, this, &LoginJob::slotLoginDone);

    return false;
}

void LoginJob::slotLoginDone()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("status")].toString() == QLatin1String("success") && replyObject.contains(QLatin1String("data"))) {
            const QJsonObject data = replyObject[QStringLiteral("data")].toObject();

            if (data.contains(QLatin1String("authToken")) && data.contains(QLatin1String("userId"))) {
                const QString authToken = data[QStringLiteral("authToken")].toString();
                const QString userId = data[QStringLiteral("userId")].toString();
                Q_EMIT loginDone(authToken, userId);
            }
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning("Error during login" + data);
        }
        reply->deleteLater();
    }
    deleteLater();
}

QJsonDocument LoginJob::json() const
{
    QVariantMap loginMap;
    loginMap.insert(QStringLiteral("user"), mUserName);
    loginMap.insert(QStringLiteral("password"), mPassword);
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest LoginJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

bool LoginJob::requireHttpAuthentication() const
{
    return false;
}

QString LoginJob::userName() const
{
    return mUserName;
}

QString LoginJob::password() const
{
    return mPassword;
}

void LoginJob::setPassword(const QString &password)
{
    mPassword = password;
}

void LoginJob::setUserName(const QString &userName)
{
    mUserName = userName;
}
