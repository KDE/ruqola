/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamremovemembersjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamRemoveMembersJob::TeamRemoveMembersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamRemoveMembersJob::~TeamRemoveMembersJob() = default;

bool TeamRemoveMembersJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamRemoveMembersJob::start");
    submitPostRequest(json());

    return true;
}

void TeamRemoveMembersJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TeamRemoveMembersJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeTeamMembersDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("TeamRemoveMembersJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QStringList TeamRemoveMembersJob::roomsId() const
{
    return mRoomsId;
}

void TeamRemoveMembersJob::setRoomsId(const QStringList &roomsId)
{
    mRoomsId = roomsId;
}

QString TeamRemoveMembersJob::teamId() const
{
    return mTeamId;
}

void TeamRemoveMembersJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

bool TeamRemoveMembersJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamRemoveMembersJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomsId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamRemoveMembersJob: mRoomsId is empty";
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamRemoveMembersJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamRemoveMembersJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsRemoveMember);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamRemoveMembersJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("rooms")] = QJsonArray::fromStringList(mRoomsId);
    jsonObj[QLatin1String("teamId")] = mTeamId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_teamremovemembersjob.cpp"
