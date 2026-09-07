/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamscreatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
TeamsCreateJob::TeamsCreateJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamsCreateJob::~TeamsCreateJob() = default;

bool TeamsCreateJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamsCreateJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void TeamsCreateJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("TeamsCreateJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT teamCreateDone(*replyObject);
    }
}

CreateChannelTeamInfo TeamsCreateJob::teamsCreateJobInfo() const
{
    return mTeamsCreateJobInfo;
}

void TeamsCreateJob::setTeamsCreateJobInfo(const CreateChannelTeamInfo &teamsCreateJobInfo)
{
    mTeamsCreateJobInfo = teamsCreateJobInfo;
}

bool TeamsCreateJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamsCreateJob::canStart() const
{
    if (mTeamsCreateJobInfo.name.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamsCreateJob: channelname is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument TeamsCreateJob::json() const
{
    return mTeamsCreateJobInfo.json();
}

QNetworkRequest TeamsCreateJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsCreate);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_teamscreatejob.cpp"
