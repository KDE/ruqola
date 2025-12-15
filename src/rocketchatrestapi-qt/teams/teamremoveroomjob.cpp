/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamremoveroomjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
TeamRemoveRoomJob::TeamRemoveRoomJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamRemoveRoomJob::~TeamRemoveRoomJob() = default;

bool TeamRemoveRoomJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamRemoveRoomJob::start");
    submitPostRequest(json());

    return true;
}

void TeamRemoveRoomJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamRemoveRoomJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeTeamRoomDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamRemoveRoomJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray TeamRemoveRoomJob::teamId() const
{
    return mTeamId;
}

void TeamRemoveRoomJob::setTeamId(const QByteArray &teamId)
{
    mTeamId = teamId;
}

QByteArray TeamRemoveRoomJob::roomId() const
{
    return mRoomId;
}

void TeamRemoveRoomJob::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

bool TeamRemoveRoomJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamRemoveRoomJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamRemoveRoomJob: mRoomId is empty";
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamRemoveRoomJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamRemoveRoomJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsRemoveRoom);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamRemoveRoomJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QLatin1StringView(mRoomId);
    jsonObj["teamId"_L1] = QLatin1StringView(mTeamId);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_teamremoveroomjob.cpp"
