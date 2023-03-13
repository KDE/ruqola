/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "markroomasreadjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
MarkRoomAsReadJob::MarkRoomAsReadJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

MarkRoomAsReadJob::~MarkRoomAsReadJob() = default;

bool MarkRoomAsReadJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("MarkRoomAsReadJob::start");
    submitPostRequest(json());

    return true;
}

void MarkRoomAsReadJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("MarkRoomAsReadJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT markAsReadDone(mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("MarkRoomAsReadJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString MarkRoomAsReadJob::roomId() const
{
    return mRoomId;
}

void MarkRoomAsReadJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool MarkRoomAsReadJob::requireHttpAuthentication() const
{
    return true;
}

bool MarkRoomAsReadJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "MarkRoomAsReadJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest MarkRoomAsReadJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsRead);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument MarkRoomAsReadJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("rid")] = mRoomId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
