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

#include "channelfilesjob.h"

#include "ruqola_restapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

ChannelFilesJob::ChannelFilesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelFilesJob::~ChannelFilesJob()
{
}

bool ChannelFilesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addLoggerInfo("ChannelFilesJob::start: ");
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &ChannelFilesJob::slotFilesinChannelFinished);
    return true;
}

void ChannelFilesJob::slotFilesinChannelFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(RUQOLA_RESTAPI_LOG) << "close channel success: " << data;
            Q_EMIT channelFilesDone(replyObject);
        } else {
            qCWarning(RUQOLA_RESTAPI_LOG) <<" Problem when we tried to find files in channel" << data;
        }
    }
    deleteLater();
}

ChannelFilesJob::ChannelType ChannelFilesJob::channelType() const
{
    return mChannelType;
}

void ChannelFilesJob::setChannelType(const ChannelType &channelType)
{
    mChannelType = channelType;
}

bool ChannelFilesJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelFilesJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChannelFilesJob: RoomId is empty";
        return false;
    }
    if (mChannelType == ChannelFilesJob::Unknown) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChannelFilesJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RUQOLA_RESTAPI_LOG) << "Impossible to start ChannelFilesJob job";
        return false;
    }
    return true;
}

QString ChannelFilesJob::roomId() const
{
    return mRoomId;
}

void ChannelFilesJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelFilesJob::request() const
{
    QUrl url;
    switch (mChannelType) {
    case Channel:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsFiles);
        break;
    case Groups:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsFiles);
        break;
    case Direct:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImFiles);
        break;
    case Unknown:
        qCWarning(RUQOLA_RESTAPI_LOG) << "ChannelFilesJob: Type is not defined";
        break;
    }
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);

    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}
