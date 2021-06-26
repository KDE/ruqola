/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

ChannelInviteJob::~ChannelInviteJob()
{
}

bool ChannelInviteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelInviteJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChannelInviteJob::slotInvitationFinished);
    return true;
}

void ChannelInviteJob::slotInvitationFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelInviteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT inviteDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelInviteJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
    if (!hasRoomIdentifier()) {
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
