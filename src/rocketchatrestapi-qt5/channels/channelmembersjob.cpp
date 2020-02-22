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

#include "channelmembersjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ChannelMembersJob::ChannelMembersJob(QObject *parent)
    : ChannelBaseJob(parent)
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
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("channelMembersDone success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelMembersDone(replyObject, channelInfo());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("channelMembersDone problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
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

bool ChannelMembersJob::hasQueryParameterSupport() const
{
    return true;
}

bool ChannelMembersJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelMembersJob::canStart() const
{
    if (!hasRoomIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: RoomId and RoomName are empty";
        return false;
    }
    if (mChannelType == ChannelMembersJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
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
    generateQuery(queryUrl);
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
