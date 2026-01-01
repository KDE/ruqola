/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetroomkeyjob.h"

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ResetRoomKeyJob::ResetRoomKeyJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ResetRoomKeyJob::~ResetRoomKeyJob() = default;

bool ResetRoomKeyJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ResetRoomKeyJob::start");
    submitPostRequest(json());
    return true;
}

void ResetRoomKeyJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ResetRoomKeyJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetRoomKeyDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ResetRoomKeyJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

ResetRoomKeyJob::ResetRoomKeyInfo ResetRoomKeyJob::resetRoomKeyInfo() const
{
    return mResetRoomKeyInfo;
}

void ResetRoomKeyJob::setResetRoomKeyInfo(const ResetRoomKeyInfo &newResetRoomKeyInfo)
{
    mResetRoomKeyInfo = newResetRoomKeyInfo;
}

bool ResetRoomKeyJob::requireHttpAuthentication() const
{
    return true;
}

QNetworkRequest ResetRoomKeyJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2EResetRoomKey);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ResetRoomKeyJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["rid"_L1] = mResetRoomKeyInfo.rid;
    jsonObj["e2eKey"_L1] = mResetRoomKeyInfo.e2eKey;
    jsonObj["e2eKeyId"_L1] = mResetRoomKeyInfo.e2eKeyId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool ResetRoomKeyJob::ResetRoomKeyInfo::isValid() const
{
    return !rid.isEmpty() && !e2eKey.isEmpty() && !e2eKeyId.isEmpty();
}
#include "moc_resetroomkeyjob.cpp"
