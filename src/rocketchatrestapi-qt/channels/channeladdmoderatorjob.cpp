/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeladdmoderatorjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ChannelAddModeratorJob::ChannelAddModeratorJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelAddModeratorJob::~ChannelAddModeratorJob() = default;

bool ChannelAddModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelAddModeratorJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelAddModeratorJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("Add moderator success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addModeratorDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("Problem when we tried to add moderator: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelAddModeratorJob::addModeratorUserId() const
{
    return mAddModeratorUserId;
}

void ChannelAddModeratorJob::setAddModeratorUserId(const QString &addModeratorUserId)
{
    mAddModeratorUserId = addModeratorUserId;
}

bool ChannelAddModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelAddModeratorJob::canStart() const
{
    if (mAddModeratorUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddModeratorJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddModeratorJob: RoomId and roomname are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelAddModeratorJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = addModeratorUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelAddModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_channeladdmoderatorjob.cpp"
