/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamdeletejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamDeleteJob::TeamDeleteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamDeleteJob::~TeamDeleteJob() = default;

bool TeamDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamDeleteJob::start");
    submitPostRequest(json());

    return true;
}

void TeamDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TeamDeleteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deleteTeamDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("TeamDeleteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QStringList TeamDeleteJob::roomsId() const
{
    return mRoomToDeleteId;
}

void TeamDeleteJob::setRoomsId(const QStringList &roomsId)
{
    mRoomToDeleteId = roomsId;
}

QString TeamDeleteJob::teamId() const
{
    return mTeamId;
}

void TeamDeleteJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

bool TeamDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamDeleteJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamDeleteJob::json() const
{
    QJsonObject jsonObj;
    if (!mRoomToDeleteId.isEmpty()) {
        jsonObj[QLatin1String("roomsToRemove")] = QJsonArray::fromStringList(mRoomToDeleteId);
    }
    jsonObj[QLatin1String("teamId")] = mTeamId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
