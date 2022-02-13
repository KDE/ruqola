/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resete2ekeyjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ResetE2EKeyJob::ResetE2EKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ResetE2EKeyJob::~ResetE2EKeyJob() = default;

bool ResetE2EKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ResetE2EKeyJob::start");
    submitPostRequest(json());
    return true;
}

void ResetE2EKeyJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ResetE2EKeyJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetAvatarDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("ResetE2EKeyJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ResetE2EKeyJob::requireHttpAuthentication() const
{
    return true;
}

bool ResetE2EKeyJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    //    if (!hasUserIdentifier()) {
    //        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ResetE2EKeyJob: identifier is empty";
    //        return false;
    //    }
    return true;
}

QNetworkRequest ResetE2EKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::UsersResetE2EKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ResetE2EKeyJob::json() const
{
    QJsonObject jsonObj;
    // generateJson(jsonObj);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool ResetE2EKeyJob::requireTwoFactorAuthentication() const
{
    return true;
}
