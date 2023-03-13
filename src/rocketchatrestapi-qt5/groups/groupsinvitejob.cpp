/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "groupsinvitejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GroupsInviteJob::GroupsInviteJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GroupsInviteJob::~GroupsInviteJob() = default;

bool GroupsInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("GroupsInviteJob::start");
    submitPostRequest(json());

    return true;
}

void GroupsInviteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GroupsInviteJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT inviteGroupsDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GroupsInviteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GroupsInviteJob::inviteUserName() const
{
    return mInviteUserName;
}

void GroupsInviteJob::setInviteUserName(const QString &inviteUserName)
{
    mInviteUserName = inviteUserName;
}

QString GroupsInviteJob::inviteUserId() const
{
    return mInviteUserId;
}

void GroupsInviteJob::setInviteUserId(const QString &userId)
{
    mInviteUserId = userId;
}

bool GroupsInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupsInviteJob::canStart() const
{
    if (mInviteUserId.isEmpty() && mInviteUserName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsInviteJob: mInviteUserId is empty or mInviteUserName is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupsInviteJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument GroupsInviteJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    if (!inviteUserId().isEmpty()) {
        jsonObj[QLatin1String("userId")] = inviteUserId();
    } else if (!inviteUserName().isEmpty()) {
        jsonObj[QLatin1String("userName")] = inviteUserName();
    }
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest GroupsInviteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsInvite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
