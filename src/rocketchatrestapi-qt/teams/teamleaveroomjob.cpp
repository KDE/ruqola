/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamleaveroomjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
TeamLeaveRoomJob::TeamLeaveRoomJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamLeaveRoomJob::~TeamLeaveRoomJob() = default;

bool TeamLeaveRoomJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamLeaveRoomJob::start");
    submitPostRequest(json());

    return true;
}

void TeamLeaveRoomJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamLeaveRoomJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeLeaveRoomDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamLeaveRoomJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString TeamLeaveRoomJob::teamId() const
{
    return mTeamId;
}

void TeamLeaveRoomJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

QString TeamLeaveRoomJob::roomId() const
{
    return mRoomId;
}

void TeamLeaveRoomJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool TeamLeaveRoomJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamLeaveRoomJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamLeaveRoomJob: mRoomId is empty";
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamLeaveRoomJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamLeaveRoomJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsLeave);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamLeaveRoomJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = mRoomId;
    jsonObj["teamId"_L1] = mTeamId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_teamleaveroomjob.cpp"
