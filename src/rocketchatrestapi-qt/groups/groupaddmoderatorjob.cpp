/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupaddmoderatorjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
GroupAddModeratorJob::GroupAddModeratorJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupAddModeratorJob::~GroupAddModeratorJob() = default;

bool GroupAddModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupAddModeratorJob::start");
    submitPostRequest(json());

    return true;
}

void GroupAddModeratorJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GroupAddModeratorJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT addModeratorDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GroupAddModeratorJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GroupAddModeratorJob::addModeratorUserId() const
{
    return mAddModeratorUserId;
}

void GroupAddModeratorJob::setAddModeratorUserId(const QString &addModeratorUserId)
{
    mAddModeratorUserId = addModeratorUserId;
}

bool GroupAddModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupAddModeratorJob::canStart() const
{
    if (mAddModeratorUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddModeratorJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddModeratorJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupAddModeratorJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = addModeratorUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupAddModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsAddModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupaddmoderatorjob.cpp"
