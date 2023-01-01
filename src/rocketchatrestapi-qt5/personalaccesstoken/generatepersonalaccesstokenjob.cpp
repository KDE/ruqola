/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generatepersonalaccesstokenjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GeneratePersonalAccessTokenJob::GeneratePersonalAccessTokenJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GeneratePersonalAccessTokenJob::~GeneratePersonalAccessTokenJob() = default;

bool GeneratePersonalAccessTokenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GeneratePersonalAccessTokenJob::start");
    submitPostRequest(json());

    return true;
}

void GeneratePersonalAccessTokenJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GeneratePersonalAccessTokenJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT generateTokenDone(replyObject);
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("GeneratePersonalAccessTokenJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GeneratePersonalAccessTokenJob::bypassTwoFactor() const
{
    return mBypassTwoFactor;
}

void GeneratePersonalAccessTokenJob::setBypassTwoFactor(bool newBypassTwoFactor)
{
    mBypassTwoFactor = newBypassTwoFactor;
}

const QString &GeneratePersonalAccessTokenJob::tokenName() const
{
    return mTokenName;
}

void GeneratePersonalAccessTokenJob::setTokenName(const QString &newTokenName)
{
    mTokenName = newTokenName;
}

bool GeneratePersonalAccessTokenJob::requireHttpAuthentication() const
{
    return true;
}

bool GeneratePersonalAccessTokenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTokenName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GeneratePersonalAccessTokenJob: mTokenName is empty";
        return false;
    }
    return true;
}

QNetworkRequest GeneratePersonalAccessTokenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersGeneratePersonalAccessToken);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument GeneratePersonalAccessTokenJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("tokenName")] = mTokenName;
    jsonObj[QLatin1String("bypassTwoFactor")] = mBypassTwoFactor;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool GeneratePersonalAccessTokenJob::requireTwoFactorAuthentication() const
{
    return true;
}
