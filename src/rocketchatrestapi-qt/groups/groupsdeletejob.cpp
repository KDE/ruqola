/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsdeletejob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
GroupsDeleteJob::GroupsDeleteJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupsDeleteJob::~GroupsDeleteJob() = default;

bool GroupsDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupsDeleteJob::start");
    submitPostRequest(json());

    return true;
}

void GroupsDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GroupsDeleteJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deleteGroupsDone(channelGroupInfo().identifier);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GroupsDeleteJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GroupsDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupsDeleteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsDeleteJob: RoomId is empty";
        return false;
    }
    return true;
}

QJsonDocument GroupsDeleteJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupsDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupsdeletejob.cpp"
