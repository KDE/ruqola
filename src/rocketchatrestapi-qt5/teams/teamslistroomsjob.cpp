/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teamslistroomsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
TeamsListRoomsJob::TeamsListRoomsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamsListRoomsJob::~TeamsListRoomsJob() = default;

bool TeamsListRoomsJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamsListRoomsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start TeamsListRoomsJob job";
        deleteLater();
        return false;
    }
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &TeamsListRoomsJob::slotTeamListRoomsFinished);
    addStartRestApiInfo(QByteArrayLiteral("TeamsListRoomsJob: ask list of rooms in team"));
    return true;
}

void TeamsListRoomsJob::slotTeamListRoomsFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamsListRoomsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT teamListRoomsDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamsListRoomsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString TeamsListRoomsJob::teamId() const
{
    return mTeamId;
}

void TeamsListRoomsJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

QNetworkRequest TeamsListRoomsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsListRooms);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("teamId"), mTeamId);
    // TODO add offset/count for the future
    // Add filter
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool TeamsListRoomsJob::canStart() const
{
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamsListRoomsJob: mTeamId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}
