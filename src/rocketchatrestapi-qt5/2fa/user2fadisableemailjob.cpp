/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "user2fadisableemailjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
User2FADisableEmailJob::User2FADisableEmailJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

User2FADisableEmailJob::~User2FADisableEmailJob() = default;

bool User2FADisableEmailJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("User2FADisableEmailJob::start");
    submitPostRequest(json());
    return true;
}

void User2FADisableEmailJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("User2FADisableEmailJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT disableEmailDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("User2FADisableEmailJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool User2FADisableEmailJob::requireHttpAuthentication() const
{
    return true;
}

bool User2FADisableEmailJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest User2FADisableEmailJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::Users2FADisableEmail);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument User2FADisableEmailJob::json() const
{
    QJsonObject jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool User2FADisableEmailJob::requireTwoFactorAuthentication() const
{
    return true;
}
