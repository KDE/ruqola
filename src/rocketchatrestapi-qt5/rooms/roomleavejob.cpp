/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomleavejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoomLeaveJob::RoomLeaveJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomLeaveJob::~RoomLeaveJob() = default;

bool RoomLeaveJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomLeaveJob::start");
    submitPostRequest(json());

    return true;
}

void RoomLeaveJob::onPostRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomLeaveJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomLeaveDone();
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomLeaveJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RoomLeaveJob::roomId() const
{
    return mRoomId;
}

void RoomLeaveJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool RoomLeaveJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomLeaveJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomLeaveJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest RoomLeaveJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsLeave);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoomLeaveJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
