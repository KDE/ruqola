/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsconverttoteamjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GroupsConvertToTeamJob::GroupsConvertToTeamJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GroupsConvertToTeamJob::~GroupsConvertToTeamJob() = default;

bool GroupsConvertToTeamJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupsConvertToTeamJob::start");
    submitPostRequest(json());

    return true;
}

void GroupsConvertToTeamJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GroupsConvertToTeamJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT groupConvertToTeamDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GroupsConvertToTeamJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GroupsConvertToTeamJob::roomId() const
{
    return mRoomId;
}

void GroupsConvertToTeamJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool GroupsConvertToTeamJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupsConvertToTeamJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsConvertToTeamJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest GroupsConvertToTeamJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsConvertToTeam);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument GroupsConvertToTeamJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

#include "moc_groupsconverttoteamjob.cpp"
