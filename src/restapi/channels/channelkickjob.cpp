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

#include "channelkickjob.h"

#include "restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ChannelKickJob::ChannelKickJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelKickJob::~ChannelKickJob()
{
}

bool ChannelKickJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChannelKickJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChannelKickJob::slotKickUsersFinished);
    return true;
}

void ChannelKickJob::slotKickUsersFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RESTAPI_LOG) << "Change announcement success: " << data;
            Q_EMIT kickUserDone(replyObject);
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to kick user: " << data;
        }
    }
    deleteLater();
}


bool ChannelKickJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelKickJob::canStart() const
{
    if (mKickUserId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChannelKickJob: mKickUserId is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChannelKickJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start ChannelKickJob job";
        return false;
    }
    return true;
}

QJsonDocument ChannelKickJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("userId")] = kickUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelKickJob::roomId() const
{
    return mRoomId;
}

void ChannelKickJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelKickJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsKick);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QString ChannelKickJob::kickUserId() const
{
    return mKickUserId;
}

void ChannelKickJob::setKickUserId(const QString &kickUserId)
{
    mKickUserId = kickUserId;
}
