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

#include "changegroupsannouncementjob.h"

#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ChangeGroupsAnnouncementJob::ChangeGroupsAnnouncementJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChangeGroupsAnnouncementJob::~ChangeGroupsAnnouncementJob()
{
}

bool ChangeGroupsAnnouncementJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChangeGroupsAnnouncementJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChangeGroupsAnnouncementJob::slotChangeGroupsannouncementFinished);
    return true;
}

void ChangeGroupsAnnouncementJob::slotChangeGroupsannouncementFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RUQOLA_RESTAPI_LOG) << "Change Topic success";
            Q_EMIT changeGroupsAnnoucement();
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to change topic" << data;
        }
    }
    deleteLater();
}

bool ChangeGroupsAnnouncementJob::requireHttpAuthentication() const
{
    return true;
}

bool ChangeGroupsAnnouncementJob::canStart() const
{
    if (mAnnouncement.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChangeGroupsAnnouncementJob: announcement is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChangeGroupsAnnouncementJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ChangeGroupsAnnouncementJob job";
        return false;
    }
    return true;
}

QJsonDocument ChangeGroupsAnnouncementJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("announcement")] = announcement();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChangeGroupsAnnouncementJob::roomId() const
{
    return mRoomId;
}

void ChangeGroupsAnnouncementJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ChangeGroupsAnnouncementJob::announcement() const
{
    return mAnnouncement;
}

void ChangeGroupsAnnouncementJob::setAnnouncement(const QString &t)
{
    mAnnouncement = t;
}

QNetworkRequest ChangeGroupsAnnouncementJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsSetAnnouncement);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
