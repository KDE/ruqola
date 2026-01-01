/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loginjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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

    if (replyObject["status"_L1].toString() == "success"_L1 && replyObject.contains("data"_L1)) {
        const QJsonObject dataObject = replyObject["data"_L1].toObject();

        if (dataObject.contains("authToken"_L1) && dataObject.contains("userId"_L1)) {
            const QString authToken = dataObject["authToken"_L1].toString();
            const QString userId = dataObject["userId"_L1].toString();
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
        loginMap.insert(u"user"_s, mUserName);
        loginMap.insert(u"password"_s, mPassword);
        if (!mCode.isEmpty()) {
            loginMap.insert(u"code"_s, mCode);
        }
    } else {
        loginMap.insert(u"resume"_s, mResume);
    }
    const QJsonDocument postData = QJsonDocument::fromVariant(loginMap);
    return postData;
}

QNetworkRequest LoginJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Login);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, u"application/json"_s);
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

#include "moc_loginjob.cpp"
