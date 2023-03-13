/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinvitejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelInviteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT inviteDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelInviteJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
        jsonObj[QLatin1String("userId")] = inviteUserId();
    } else if (!inviteUserName().isEmpty()) {
        jsonObj[QLatin1String("userName")] = inviteUserName();
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
