/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamslistroomsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
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

    addStartRestApiInfo("TeamsListRoomsJob: ask list of rooms in team"_ba);
    return true;
}

void TeamsListRoomsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("TeamsListRoomsJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamListRoomsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("TeamsListRoomsJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QByteArray TeamsListRoomsJob::teamId() const
{
    return mTeamId;
}

void TeamsListRoomsJob::setTeamId(const QByteArray &teamId)
{
    mTeamId = teamId;
}

QNetworkRequest TeamsListRoomsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsListRooms);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"teamId"_s, QLatin1StringView(mTeamId));
    // TODO add offset/count for the future
    // Add filter
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool TeamsListRoomsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamsListRoomsJob: mTeamId is empty";
        return false;
    }
    return true;
}

QString TeamsListRoomsJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == "team-does-not-exist"_L1) {
        return i18n("The required \\\"roomId\\\" or \\\"roomName\\\" param provided does not match any teams");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

#include "moc_teamslistroomsjob.cpp"
