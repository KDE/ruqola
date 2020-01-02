/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QUrlQuery>
#include <QJsonObject>
#include <QNetworkReply>

using namespace RocketChatRestApi;
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetChannelRolesJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetChannelRolesJob job";
        return false;
    }
    return true;
}

bool GetChannelRolesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
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
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRoles);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    addAuthRawHeader(request);
    return request;
}

bool GetChannelRolesJob::requireHttpAuthentication() const
{
    return true;
}

void GetChannelRolesJob::slotGetChannelRolesFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetChannelRolesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelRolesDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("GetChannelRolesJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
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

QString RocketChatRestApi::GetChannelRolesJob::jobName() const
{
    return i18n("Extract Roles");
}
