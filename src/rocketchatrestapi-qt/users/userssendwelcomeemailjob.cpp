/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userssendwelcomeemailjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UsersSendWelcomeEmailJob::UsersSendWelcomeEmailJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UsersSendWelcomeEmailJob::~UsersSendWelcomeEmailJob() = default;

bool UsersSendWelcomeEmailJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UsersSendWelcomeEmailJob::start");
    submitPostRequest(json());
    return true;
}

void UsersSendWelcomeEmailJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("UsersSendWelcomeEmailJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT sendWelcomeEmailDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UsersSendWelcomeEmailJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString UsersSendWelcomeEmailJob::email() const
{
    return mEmail;
}

void UsersSendWelcomeEmailJob::setEmail(const QString &newEmail)
{
    mEmail = newEmail;
}

bool UsersSendWelcomeEmailJob::requireHttpAuthentication() const
{
    return true;
}

bool UsersSendWelcomeEmailJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mEmail.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Any email defined";
        return false;
    }
    return true;
}

QNetworkRequest UsersSendWelcomeEmailJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersSendWelcomeEmail);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UsersSendWelcomeEmailJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["email"_L1] = mEmail;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_userssendwelcomeemailjob.cpp"
