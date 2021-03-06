/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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

#include "channelhistoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ChannelHistoryJob::ChannelHistoryJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelHistoryJob::~ChannelHistoryJob()
{
}

bool ChannelHistoryJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelHistoryJob::start");
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &ChannelHistoryJob::slotLoadHistoryChannelFinished);
    return true;
}

void ChannelHistoryJob::slotLoadHistoryChannelFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelHistoryJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelHistoryDone(replyObject, channelGroupInfo());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelHistoryJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

ChannelHistoryJob::ChannelHistoryInfo ChannelHistoryJob::channelHistoryInfo() const
{
    return mChannelHistoryInfo;
}

void ChannelHistoryJob::setChannelHistoryInfo(const ChannelHistoryInfo &channelHistoryInfo)
{
    mChannelHistoryInfo = channelHistoryInfo;
}

bool ChannelHistoryJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelHistoryJob::canStart() const
{
    //    if (!hasIdentifier()) {
    //        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: RoomId and RoomName are empty";
    //        return false;
    //    }
    if (mChannelHistoryInfo.channelType == ChannelHistoryJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: Channel type is unknown.";
        return false;
    }
    if (mChannelHistoryInfo.roomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: roomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest ChannelHistoryJob::request() const
{
    QUrl url;
    switch (mChannelHistoryInfo.channelType) {
    case Channel:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsHistory);
        break;
    case Groups:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsHistory);
        break;
    case Direct:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImHistory);
        break;
    case Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: Type is not defined";
        break;
    }

    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mChannelHistoryInfo.roomId);
    if (!mChannelHistoryInfo.latestMessage.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("latest"), mChannelHistoryInfo.latestMessage);
    }
    if (!mChannelHistoryInfo.oldestMessage.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("oldest"), mChannelHistoryInfo.oldestMessage);
    }
    if (mChannelHistoryInfo.offset > 0) {
        queryUrl.addQueryItem(QStringLiteral("offset"), QString::number(mChannelHistoryInfo.offset));
    }
    if (mChannelHistoryInfo.count > 0) {
        queryUrl.addQueryItem(QStringLiteral("count"), QString::number(mChannelHistoryInfo.count));
    }
    queryUrl.addQueryItem(QStringLiteral("inclusive"), mChannelHistoryInfo.inclusive ? QStringLiteral("true") : QStringLiteral("false"));
    queryUrl.addQueryItem(QStringLiteral("unreads"), mChannelHistoryInfo.unreads ? QStringLiteral("true") : QStringLiteral("false"));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &t)
{
    d << "latestMessage " << t.latestMessage;
    d << "oldestMessage " << t.oldestMessage;
    d << "channelType " << t.channelType;
    d << "offset " << t.offset;
    d << "count " << t.count;
    d << "inclusive " << t.inclusive;
    d << "unreads " << t.unreads;
    d << "roomId " << t.roomId;
    return d;
}
