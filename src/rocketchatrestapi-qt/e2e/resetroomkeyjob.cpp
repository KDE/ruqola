/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "resetroomkeyjob.h"

#include "restapimethod.h"

#include <QJsonDocument>
#include <QJsonObject>
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
        addLoggerInfo(QByteArrayLiteral("ResetRoomKeyJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT resetRoomKeyDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ResetRoomKeyJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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
    // TODO
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_resetroomkeyjob.cpp"
