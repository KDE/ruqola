/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupremoveleaderjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GroupRemoveLeaderJob::GroupRemoveLeaderJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupRemoveLeaderJob::~GroupRemoveLeaderJob() = default;

bool GroupRemoveLeaderJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupRemoveLeaderJob::start");
    submitPostRequest(json());

    return true;
}

void GroupRemoveLeaderJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GroupRemoveLeaderJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT removeLeaderDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GroupRemoveLeaderJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GroupRemoveLeaderJob::removeUserId() const
{
    return mRemoveUserId;
}

void GroupRemoveLeaderJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool GroupRemoveLeaderJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupRemoveLeaderJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveLeaderJob: remove userid is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupRemoveLeaderJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupRemoveLeaderJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj[QLatin1String("userId")] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupRemoveLeaderJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupRemoveLeader);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_groupremoveleaderjob.cpp"
