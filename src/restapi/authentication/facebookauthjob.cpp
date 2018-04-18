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
#include "ruqola_restapi_debug.h"
#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

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
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start login job";
        return false;
    }
    if (mUserName.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "UserName is empty";
        return false;
    }
    if (mPassword.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Password is empty";
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
    addLoggerInfo("FacebookAuthJob started ");
    connect(reply, &QNetworkReply::finished, this, &FacebookAuthJob::slotLoginDone);

    return false;
}

void FacebookAuthJob::slotLoginDone()
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
                Q_EMIT loginDone(authToken, userId);
            }
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) << "Error during login" << data;
        }
    }
    deleteLater();
}

QJsonDocument FacebookAuthJob::json() const
{
    QVariantMap loginMap;
    loginMap.insert(QStringLiteral("user"), mUserName);
    loginMap.insert(QStringLiteral("password"), mPassword);
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

QString FacebookAuthJob::userName() const
{
    return mUserName;
}

QString FacebookAuthJob::password() const
{
    return mPassword;
}

void FacebookAuthJob::setPassword(const QString &password)
{
    mPassword = password;
}

void FacebookAuthJob::setUserName(const QString &userName)
{
    mUserName = userName;
}
