/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupaddleaderjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
GroupAddLeaderJob::GroupAddLeaderJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupAddLeaderJob::~GroupAddLeaderJob() = default;

bool GroupAddLeaderJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupAddLeaderJob::start");
    submitPostRequest(json());

    return true;
}

void GroupAddLeaderJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GroupAddLeaderJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addLeaderDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GroupAddLeaderJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GroupAddLeaderJob::addLeaderUserId() const
{
    return mAddLeaderUserId;
}

void GroupAddLeaderJob::setAddLeaderUserId(const QString &addownerUserId)
{
    mAddLeaderUserId = addownerUserId;
}

bool GroupAddLeaderJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupAddLeaderJob::canStart() const
{
    if (mAddLeaderUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddLeaderJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddLeaderJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupAddLeaderJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = addLeaderUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupAddLeaderJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsAddLeader);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupaddleaderjob.cpp"
