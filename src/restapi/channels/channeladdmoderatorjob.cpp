/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "channeladdmoderatorjob.h"

#include "restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelAddModeratorJob::ChannelAddModeratorJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelAddModeratorJob::~ChannelAddModeratorJob()
{
}

bool ChannelAddModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChannelAddModeratorJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChannelAddModeratorJob::slotAddModeratorFinished);
    return true;
}

void ChannelAddModeratorJob::slotAddModeratorFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RESTAPI_LOG) << "Add owner success: " << data;
            Q_EMIT addModeratorDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to add moderator : " << data;
        }
    }
    deleteLater();
}

QString ChannelAddModeratorJob::addModeratorUserId() const
{
    return mAddModeratorUserId;
}

void ChannelAddModeratorJob::setAddModeratorUserId(const QString &addModeratorUserId)
{
    mAddModeratorUserId = addModeratorUserId;
}

bool ChannelAddModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelAddModeratorJob::canStart() const
{
    if (mAddModeratorUserId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChannelAddModeratorJob: remove userid is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChannelAddModeratorJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start ChannelAddModeratorJob job";
        return false;
    }
    return true;
}

QJsonDocument ChannelAddModeratorJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("userId")] = addModeratorUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelAddModeratorJob::roomId() const
{
    return mRoomId;
}

void ChannelAddModeratorJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelAddModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
