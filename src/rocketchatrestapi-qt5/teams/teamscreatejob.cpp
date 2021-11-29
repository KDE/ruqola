/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamscreatejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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
    addStartRestApiInfo("TeamsCreateJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TeamsCreateJob::slotTeamCreateFinished);
    return true;
}

void TeamsCreateJob::slotTeamCreateFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamsCreateJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT teamCreateDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamsCreateJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

CreateRoomInfo TeamsCreateJob::teamsCreateJobInfo() const
{
    return mTeamsCreateJobInfo;
}

void TeamsCreateJob::setTeamsCreateJobInfo(const CreateRoomInfo &teamsCreateJobInfo)
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
