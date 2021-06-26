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

#include "channelfilesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelFilesJob::ChannelFilesJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
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
    addStartRestApiInfo("ChannelFilesJob::start: ");
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &ChannelFilesJob::slotFilesinChannelFinished);
    return true;
}

void ChannelFilesJob::slotFilesinChannelFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("channelFilesDone success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelFilesDone(replyObject, channelInfo());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("channelFilesDone problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

ChannelFilesJob::ChannelType ChannelFilesJob::channelType() const
{
    return mChannelType;
}

void ChannelFilesJob::setChannelType(ChannelType channelType)
{
    mChannelType = channelType;
}

bool ChannelFilesJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelFilesJob::canStart() const
{
    if (!hasRoomIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelFilesJob: RoomId and RoomName are empty";
        return false;
    }
    if (mChannelType == ChannelFilesJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelFilesJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
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
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelFilesJob: Type is not defined";
        break;
    }
    addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool ChannelFilesJob::hasQueryParameterSupport() const
{
    return true;
}
