/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changegroupstopicjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
ChangeGroupsTopicJob::ChangeGroupsTopicJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChangeGroupsTopicJob::~ChangeGroupsTopicJob() = default;

bool ChangeGroupsTopicJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChangeGroupsTopicJob::start");
    submitPostRequest(json());

    return true;
}

void ChangeGroupsTopicJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChangeGroupsTopicJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT changeTopicDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChangeGroupsTopicJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChangeGroupsTopicJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsTopicJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChangeGroupsTopicJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsTopicJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["topic"_L1] = topic();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeGroupsTopicJob::topic() const
{
    return mTopic;
}

void ChangeGroupsTopicJob::setTopic(const QString &topic)
{
    mTopic = topic;
}

QNetworkRequest ChangeGroupsTopicJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetTopic);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_changegroupstopicjob.cpp"
