/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremovemoderatorjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
GroupRemoveModeratorJob::GroupRemoveModeratorJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupRemoveModeratorJob::~GroupRemoveModeratorJob() = default;

bool GroupRemoveModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupRemoveModeratorJob::start");
    submitPostRequest(json());

    return true;
}

void GroupRemoveModeratorJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GroupRemoveModeratorJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeModeratorDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GroupRemoveModeratorJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GroupRemoveModeratorJob::removeUserId() const
{
    return mRemoveUserId;
}

void GroupRemoveModeratorJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool GroupRemoveModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupRemoveModeratorJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveModeratorJob: remove userid is empty";
        return false;
    }
    if (hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveModeratorJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupRemoveModeratorJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupRemoveModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsRemoveModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupremovemoderatorjob.cpp"
