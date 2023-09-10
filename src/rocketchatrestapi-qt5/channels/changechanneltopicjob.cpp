/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changechanneltopicjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeChannelTopicJob::ChangeChannelTopicJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelTopicJob::~ChangeChannelTopicJob() = default;

bool ChangeChannelTopicJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeChannelTopicJob::start");
    submitPostRequest(json());
    return true;
}

void ChangeChannelTopicJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("Change Topic success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeTopicDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("Problem when we tried to change topic: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeChannelTopicJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelTopicJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeChannelTopicJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelTopicJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("topic")] = topic();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelTopicJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelTopicJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeChannelTopicJob::topic() const
{
    return mTopic;
}

void ChangeChannelTopicJob::setTopic(const QString &topic)
{
    mTopic = topic;
}

QNetworkRequest ChangeChannelTopicJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetTopic);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changechanneltopicjob.cpp"
