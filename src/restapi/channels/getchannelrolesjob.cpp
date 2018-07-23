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

#include "getchannelrolesjob.h"
#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

GetChannelRolesJob::GetChannelRolesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetChannelRolesJob::~GetChannelRolesJob()
{
}

bool GetChannelRolesJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "GetChannelRolesJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start GetChannelRolesJob job";
        return false;
    }
    return true;
}

bool GetChannelRolesJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo("GetChannelRolesJob::start");
    connect(reply, &QNetworkReply::finished, this, &GetChannelRolesJob::slotGetChannelRolesFinished);

    return true;
}

QNetworkRequest GetChannelRolesJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRoles);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

bool GetChannelRolesJob::requireHttpAuthentication() const
{
    return false;
}

void GetChannelRolesJob::slotGetChannelRolesFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        addLoggerInfo(QByteArrayLiteral("GetChannelRolesJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelRolesDone(replyObject);
    }
    deleteLater();
}

QString GetChannelRolesJob::roomId() const
{
    return mRoomId;
}

void GetChannelRolesJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}
