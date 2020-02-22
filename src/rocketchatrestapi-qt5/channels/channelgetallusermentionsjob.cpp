/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
#else
    request.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
#endif
    return request;
}

bool ChannelGetAllUserMentionsJob::requireHttpAuthentication() const
{
    return true;
}

void ChannelGetAllUserMentionsJob::slotChannelGetAllUserMentionsFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelGetAllUserMentionsJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelGetAllUserMentionsDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelGetAllUserMentionsJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
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

bool RocketChatRestApi::ChannelGetAllUserMentionsJob::hasQueryParameterSupport() const
{
    return true;
}
