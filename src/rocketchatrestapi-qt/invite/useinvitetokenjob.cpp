/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "useinvitetokenjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
UseInviteTokenJob::UseInviteTokenJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

UseInviteTokenJob::~UseInviteTokenJob() = default;

bool UseInviteTokenJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("UseInviteTokenJob::start");
    submitPostRequest(json());
    return true;
}

void UseInviteTokenJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("UseInviteTokenJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT useInviteTokenDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("UseInviteTokenJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString UseInviteTokenJob::token() const
{
    return mToken;
}

void UseInviteTokenJob::setToken(const QString &newToken)
{
    mToken = newToken;
}

bool UseInviteTokenJob::requireHttpAuthentication() const
{
    return true;
}

bool UseInviteTokenJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mToken.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "token not defined";
        return false;
    }
    return true;
}

QNetworkRequest UseInviteTokenJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UseInviteToken);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument UseInviteTokenJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["token"_L1] = mToken;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_useinvitetokenjob.cpp"
