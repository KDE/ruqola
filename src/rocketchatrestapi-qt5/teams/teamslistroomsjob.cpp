/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamslistroomsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
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
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("TeamsListRoomsJob: ask list of rooms in team"));
    return true;
}

void TeamsListRoomsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TeamsListRoomsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamListRoomsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("TeamsListRoomsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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

QString TeamsListRoomsJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1String("team-does-not-exist")) {
        return i18n("The required \\\"roomId\\\" or \\\"roomName\\\" param provided does not match any teams");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

#include "moc_teamslistroomsjob.cpp"
