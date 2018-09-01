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

#include "changechannelannouncementjob.h"

#include "restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChangeChannelAnnouncementJob::ChangeChannelAnnouncementJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeChannelAnnouncementJob::~ChangeChannelAnnouncementJob()
{
}

bool ChangeChannelAnnouncementJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChangeChannelAnnouncementJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChangeChannelAnnouncementJob::slotChangeAnnouncementFinished);
    return true;
}

void ChangeChannelAnnouncementJob::slotChangeAnnouncementFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RESTAPI_LOG) << "Change announcement success: " << data;
            Q_EMIT changeAnnouncementDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to change announcement: " << data;
        }
    }
    deleteLater();
}

bool ChangeChannelAnnouncementJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeChannelAnnouncementJob::canStart() const
{
    if (mAnnouncement.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChangeChannelAnnouncementJob: announcement is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "ChangeChannelAnnouncementJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start ChangeChannelAnnouncementJob job";
        return false;
    }
    return true;
}

QJsonDocument ChangeChannelAnnouncementJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("announcement")] = announcement();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeChannelAnnouncementJob::roomId() const
{
    return mRoomId;
}

void ChangeChannelAnnouncementJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeChannelAnnouncementJob::announcement() const
{
    return mAnnouncement;
}

void ChangeChannelAnnouncementJob::setAnnouncement(const QString &t)
{
    mAnnouncement = t;
}

QNetworkRequest ChangeChannelAnnouncementJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetAnnouncement);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
