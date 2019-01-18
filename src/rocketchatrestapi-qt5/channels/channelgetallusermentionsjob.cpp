/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "channelgetallusermentionsjob.h"
#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QUrlQuery>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelGetAllUserMentionsJob::ChannelGetAllUserMentionsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelGetAllUserMentionsJob::~ChannelGetAllUserMentionsJob()
{
}

bool ChannelGetAllUserMentionsJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelGetAllUserMentionsJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelGetAllUserMentionsJob job";
        return false;
    }
    return true;
}

bool ChannelGetAllUserMentionsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = mNetworkAccessManager->get(request());
    addLoggerInfo("ChannelGetAllUserMentionsJob::start");
    connect(reply, &QNetworkReply::finished, this, &ChannelGetAllUserMentionsJob::slotChannelGetAllUserMentionsFinished);

    return true;
}

QNetworkRequest ChannelGetAllUserMentionsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsGetAllUserMentionsByChannel);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    return request;
}

bool ChannelGetAllUserMentionsJob::requireHttpAuthentication() const
{
    return true;
}

void ChannelGetAllUserMentionsJob::slotChannelGetAllUserMentionsFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        addLoggerInfo(QByteArrayLiteral("ChannelGetAllUserMentionsJob: finished: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelGetAllUserMentionsDone(replyObject);
    }
    deleteLater();
}

QString ChannelGetAllUserMentionsJob::roomId() const
{
    return mRoomId;
}

void ChannelGetAllUserMentionsJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}
