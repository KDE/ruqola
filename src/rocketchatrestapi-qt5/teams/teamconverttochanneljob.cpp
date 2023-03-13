/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochanneljob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamConvertToChannelJob::TeamConvertToChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamConvertToChannelJob::~TeamConvertToChannelJob() = default;

bool TeamConvertToChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamConvertToChannelJob::start");
    submitPostRequest(json());

    return true;
}

void TeamConvertToChannelJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("TeamConvertToChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT teamConvertToChannelDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("TeamConvertToChannelJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QStringList &TeamConvertToChannelJob::roomsToRemove() const
{
    return mRoomsToRemove;
}

void TeamConvertToChannelJob::setRoomsToRemove(const QStringList &newRoomsToRemove)
{
    mRoomsToRemove = newRoomsToRemove;
}

QString TeamConvertToChannelJob::teamId() const
{
    return mTeamId;
}

void TeamConvertToChannelJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

bool TeamConvertToChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamConvertToChannelJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamConvertToChannelJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamConvertToChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsConvertToChannel);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamConvertToChannelJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("teamId")] = mTeamId;
    if (!mRoomsToRemove.isEmpty()) {
        jsonObj[QLatin1String("roomsToRemove")] = QJsonArray::fromStringList(mRoomsToRemove);
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
