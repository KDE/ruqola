/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamaddroomsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamAddRoomsJob::TeamAddRoomsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamAddRoomsJob::~TeamAddRoomsJob() = default;

bool TeamAddRoomsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamAddRoomsJob::start");
    submitPostRequest(json());

    return true;
}

void TeamAddRoomsJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TeamAddRoomsJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamAddRoomsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("TeamAddRoomsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QStringList TeamAddRoomsJob::roomIds() const
{
    return mRoomIds;
}

void TeamAddRoomsJob::setRoomIds(const QStringList &roomsId)
{
    mRoomIds = roomsId;
}

QString TeamAddRoomsJob::teamId() const
{
    return mTeamId;
}

void TeamAddRoomsJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

bool TeamAddRoomsJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamAddRoomsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomIds.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamAddRoomsJob: mRoomsId is empty";
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamAddRoomsJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamAddRoomsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsAddRooms);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamAddRoomsJob::json() const
{
    QJsonObject jsonObj;
    // Example {"rooms":["QMkvkiMyxKoEuJjnb","zMHhMfsEPvKjgFuyE"],"teamId":"6072f51066b377a354d793cc"}
    jsonObj[QLatin1String("rooms")] = QJsonArray::fromStringList(mRoomIds);
    jsonObj[QLatin1String("teamId")] = mTeamId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString TeamAddRoomsJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1String("error-no-permission-team-channel")) {
        return i18n("You don't have permission to add this channel to the team");
    }
    return RestApiAbstractJob::generateErrorMessage(errorStr);
}

#include "moc_teamaddroomsjob.cpp"
