/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetowne2ekeyjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ResetOwnE2eKeyJob::ResetOwnE2eKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ResetOwnE2eKeyJob::~ResetOwnE2eKeyJob() = default;

bool ResetOwnE2eKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ResetOwnE2eKeyJob::start");
    submitPostRequest(json());
    return true;
}

void ResetOwnE2eKeyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ResetOwnE2eKeyJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetE2eKeyDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ResetOwnE2eKeyJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ResetOwnE2eKeyJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest ResetOwnE2eKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EResetOwnE2EKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ResetOwnE2eKeyJob::json() const
{
    QJsonObject jsonObj;
    // TODO
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_resetowne2ekeyjob.cpp"
