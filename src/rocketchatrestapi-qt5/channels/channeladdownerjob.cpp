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

#include "channeladdownerjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelAddOwnerJob::ChannelAddOwnerJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelAddOwnerJob::~ChannelAddOwnerJob()
{
}

bool ChannelAddOwnerJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChannelAddOwnerJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChannelAddOwnerJob::slotAddOwnerFinished);
    return true;
}

void ChannelAddOwnerJob::slotAddOwnerFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << "Add owner success: " << data;
            Q_EMIT addOwnerDone();
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) <<" Problem when we tried to add owner : " << data;
        }
    }
    deleteLater();
}

QString ChannelAddOwnerJob::addownerUserId() const
{
    return mAddownerUserId;
}

void ChannelAddOwnerJob::setAddownerUserId(const QString &addownerUserId)
{
    mAddownerUserId = addownerUserId;
}

bool ChannelAddOwnerJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelAddOwnerJob::canStart() const
{
    if (mAddownerUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddOwnerJob: remove userid is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelAddOwnerJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelAddOwnerJob job";
        return false;
    }
    return true;
}

QJsonDocument ChannelAddOwnerJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("userId")] = addownerUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelAddOwnerJob::roomId() const
{
    return mRoomId;
}

void ChannelAddOwnerJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelAddOwnerJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddOwner);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
