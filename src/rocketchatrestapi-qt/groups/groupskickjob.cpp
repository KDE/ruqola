/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupskickjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
GroupsKickJob::GroupsKickJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupsKickJob::~GroupsKickJob() = default;

bool GroupsKickJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupsKickJob::start");
    submitPostRequest(json());

    return true;
}

void GroupsKickJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GroupsKickJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT kickUserDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GroupsKickJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GroupsKickJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupsKickJob::canStart() const
{
    if (mKickUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsKickJob: mKickUserId is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsKickJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupsKickJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = QLatin1StringView(kickUserId());

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupsKickJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsKick);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QByteArray GroupsKickJob::kickUserId() const
{
    return mKickUserId;
}

void GroupsKickJob::setKickUserId(const QByteArray &kickUserId)
{
    mKickUserId = kickUserId;
}

#include "moc_groupskickjob.cpp"
