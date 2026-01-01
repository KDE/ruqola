/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "regeneratepersonalaccesstokenjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RegeneratePersonalAccessTokenJob::RegeneratePersonalAccessTokenJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RegeneratePersonalAccessTokenJob::~RegeneratePersonalAccessTokenJob() = default;

bool RegeneratePersonalAccessTokenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RegeneratePersonalAccessTokenJob::start");
    submitPostRequest(json());

    return true;
}

void RegeneratePersonalAccessTokenJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RegeneratePersonalAccessTokenJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT regenerateTokenDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RegeneratePersonalAccessTokenJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &RegeneratePersonalAccessTokenJob::tokenName() const
{
    return mTokenName;
}

void RegeneratePersonalAccessTokenJob::setTokenName(const QString &newTokenName)
{
    mTokenName = newTokenName;
}

bool RegeneratePersonalAccessTokenJob::requireHttpAuthentication() const
{
    return true;
}

bool RegeneratePersonalAccessTokenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTokenName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RegeneratePersonalAccessTokenJob: mTokenName is empty";
        return false;
    }
    return true;
}

QNetworkRequest RegeneratePersonalAccessTokenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersRegeneratePersonalAccessToken);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RegeneratePersonalAccessTokenJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["tokenName"_L1] = mTokenName;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool RegeneratePersonalAccessTokenJob::requireTwoFactorAuthentication() const
{
    return true;
}

#include "moc_regeneratepersonalaccesstokenjob.cpp"
