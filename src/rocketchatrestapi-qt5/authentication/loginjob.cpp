/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loginjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LoginJob::LoginJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LoginJob::~LoginJob() = default;

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
    submitPostRequest(json());
    return true;
}

void LoginJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("status")].toString() == QLatin1String("success") && replyObject.contains(QLatin1String("data"))) {
        const QJsonObject dataObject = replyObject[QStringLiteral("data")].toObject();

        if (dataObject.contains(QLatin1String("authToken")) && dataObject.contains(QLatin1String("userId"))) {
            const QString authToken = dataObject[QStringLiteral("authToken")].toString();
            const QString userId = dataObject[QStringLiteral("userId")].toString();
            Q_EMIT loginDone(authToken, userId);
        }
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Error during login" + replyJson.toJson(QJsonDocument::Indented));
    }
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
