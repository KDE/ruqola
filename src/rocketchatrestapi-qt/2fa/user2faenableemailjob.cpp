/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2faenableemailjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
User2FAEnableEmailJob::User2FAEnableEmailJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

User2FAEnableEmailJob::~User2FAEnableEmailJob() = default;

bool User2FAEnableEmailJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("User2FAEnableEmail::start");
    submitPostRequest(json());
    return true;
}

void User2FAEnableEmailJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("User2FAEnableEmail: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT enableEmailDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("User2FAEnableEmail: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool User2FAEnableEmailJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest User2FAEnableEmailJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Users2FAEnableEmail);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument User2FAEnableEmailJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_user2faenableemailjob.cpp"
