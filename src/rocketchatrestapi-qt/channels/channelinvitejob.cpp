/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinvitejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ChannelInviteJob::ChannelInviteJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelInviteJob::~ChannelInviteJob() = default;

bool ChannelInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelInviteJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelInviteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelInviteJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT inviteDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelInviteJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelInviteJob::inviteUserName() const
{
    return mInviteUserName;
}

void ChannelInviteJob::setInviteUserName(const QString &userName)
{
    mInviteUserName = userName;
}

QString ChannelInviteJob::inviteUserId() const
{
    return mInviteUserId;
}

void ChannelInviteJob::setInviteUserId(const QString &userId)
{
    mInviteUserId = userId;
}

bool ChannelInviteJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelInviteJob::canStart() const
{
    if (mInviteUserId.isEmpty() && mInviteUserName.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInviteJob: inviteUserId is empty or inviteUserName is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInviteJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelInviteJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    if (!inviteUserId().isEmpty()) {
        jsonObj["userId"_L1] = inviteUserId();
    } else if (!inviteUserName().isEmpty()) {
        jsonObj["userName"_L1] = inviteUserName();
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelInviteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsInvite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_channelinvitejob.cpp"
