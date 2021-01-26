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
using namespace RocketChatRestApi;
ChannelHistoryJob::ChannelHistoryJob(QObject *parent)
    : ChannelBaseJob(parent)
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
    QNetworkReply *reply = submitPostRequest(json());
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
            Q_EMIT channelHistoryDone();
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
    if (!hasRoomIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: RoomId and RoomName are empty";
        return false;
    }
    if (mChannelHistoryInfo.channelType == ChannelHistoryJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelHistoryJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
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
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
