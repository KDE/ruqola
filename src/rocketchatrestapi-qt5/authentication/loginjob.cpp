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
        return false;
    }
    if (!mResume.isEmpty()) {
        return true;
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
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &LoginJob::slotLoginDone);
    return true;
}

void LoginJob::slotLoginDone()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("status")].toString() == QLatin1String("success") && replyObject.contains(QLatin1String("data"))) {
            const QJsonObject dataObject = replyObject[QStringLiteral("data")].toObject();

            if (dataObject.contains(QLatin1String("authToken")) && dataObject.contains(QLatin1String("userId"))) {
                const QString authToken = dataObject[QStringLiteral("authToken")].toString();
                const QString userId = dataObject[QStringLiteral("userId")].toString();
                Q_EMIT loginDone(authToken, userId);
            }
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning("Error during login" + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString LoginJob::code() const
{
    return mCode;
}

void LoginJob::setCode(const QString &code)
{
    mCode = code;
}

QString LoginJob::resume() const
{
    return mResume;
}

void LoginJob::setResume(const QString &resume)
{
    mResume = resume;
}

QJsonDocument LoginJob::json() const
{
    QVariantMap loginMap;
    if (mResume.isEmpty()) {
        loginMap.insert(QStringLiteral("user"), mUserName);
        loginMap.insert(QStringLiteral("password"), mPassword);
        if (!mCode.isEmpty()) {
            loginMap.insert(QStringLiteral("code"), mCode);
        }
    } else {
        loginMap.insert(QStringLiteral("resume"), mResume);
    }
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
