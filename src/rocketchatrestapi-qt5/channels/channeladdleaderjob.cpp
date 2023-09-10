/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeladdleaderjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelAddLeaderJob::ChannelAddLeaderJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelAddLeaderJob::~ChannelAddLeaderJob() = default;

bool ChannelAddLeaderJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelAddLeaderJob::start");
    submitPostRequest(json());
    return true;
}

void ChannelAddLeaderJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelAddLeaderJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addLeaderDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelAddLeaderJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelAddLeaderJob::addLeaderUserId() const
{
    return mAddLeaderUserId;
}

void ChannelAddLeaderJob::setAddLeaderUserId(const QString &addownerUserId)
{
    mAddLeaderUserId = addownerUserId;
}

bool ChannelAddLeaderJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelAddLeaderJob::canStart() const
{
    if (mAddLeaderUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddLeaderJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddLeaderJob: RoomId or RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelAddLeaderJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("userId")] = addLeaderUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelAddLeaderJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddLeader);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_channeladdleaderjob.cpp"
