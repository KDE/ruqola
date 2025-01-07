/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "validateinvitetokenjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ValidateInviteTokenJob::ValidateInviteTokenJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ValidateInviteTokenJob::~ValidateInviteTokenJob() = default;

bool ValidateInviteTokenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ValidateInviteTokenJob::start");
    submitPostRequest(json());
    return true;
}

void ValidateInviteTokenJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ValidateInviteTokenJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT validateInviteTokenDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ValidateInviteTokenJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT inviteTokenInvalid();
    }
}

QString ValidateInviteTokenJob::token() const
{
    return mToken;
}

void ValidateInviteTokenJob::setToken(const QString &newToken)
{
    mToken = newToken;
}

bool ValidateInviteTokenJob::requireHttpAuthentication() const
{
    return false;
}

bool ValidateInviteTokenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mToken.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Token not defined";
        return false;
    }
    return true;
}

QNetworkRequest ValidateInviteTokenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ValidateInviteToken);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ValidateInviteTokenJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["token"_L1] = mToken;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_validateinvitetokenjob.cpp"
