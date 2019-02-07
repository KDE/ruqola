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

#include "channelmembersjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ChannelMembersJob::ChannelMembersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelMembersJob::~ChannelMembersJob()
{
}

bool ChannelMembersJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addLoggerInfo("ChannelMembersJob::start: ");
    QNetworkReply *reply = mNetworkAccessManager->get(request());
    connect(reply, &QNetworkReply::finished, this, &ChannelMembersJob::slotChannelMembersFinished);
    return true;
}

void ChannelMembersJob::slotChannelMembersFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("channelMembersDone success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelMembersDone(replyObject, mRoomId);
        } else {
            addLoggerWarning(QByteArrayLiteral("channelMembersDone problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

ChannelMembersJob::ChannelType ChannelMembersJob::channelType() const
{
    return mChannelType;
}

void ChannelMembersJob::setChannelType(ChannelType channelType)
{
    mChannelType = channelType;
}

bool ChannelMembersJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelMembersJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: RoomId is empty";
        return false;
    }
    if (mChannelType == ChannelMembersJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelMembersJob job";
        return false;
    }
    return true;
}

QString ChannelMembersJob::roomId() const
{
    return mRoomId;
}

void ChannelMembersJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelMembersJob::request() const
{
    QUrl url;
    switch (mChannelType) {
    case Channel:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsMembers);
        break;
    case Groups:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsMembers);
        break;
    case Direct:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImMembers);
        break;
    case Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: Type is not defined";
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
