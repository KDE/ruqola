/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "channelsconverttoteamjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelsConvertToTeamJob::ChannelsConvertToTeamJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelsConvertToTeamJob::~ChannelsConvertToTeamJob()
{
}

bool ChannelsConvertToTeamJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelsConvertToTeamJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChannelsConvertToTeamJob::slotChannelConvertToTeamFinished);
    return true;
}

void ChannelsConvertToTeamJob::slotChannelConvertToTeamFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelsConvertToTeamJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelConvertToTeamDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelsConvertToTeamJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QStringList ChannelsConvertToTeamJob::roomIds() const
{
    return mRoomIds;
}

void ChannelsConvertToTeamJob::setRoomIds(const QStringList &roomsId)
{
    mRoomIds = roomsId;
}

QString ChannelsConvertToTeamJob::teamId() const
{
    return mTeamId;
}

void ChannelsConvertToTeamJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

bool ChannelsConvertToTeamJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelsConvertToTeamJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomIds.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelsConvertToTeamJob: mRoomsId is empty";
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelsConvertToTeamJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest ChannelsConvertToTeamJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsConvertToTeam);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ChannelsConvertToTeamJob::json() const
{
    QJsonObject jsonObj;
    // Example {"rooms":["QMkvkiMyxKoEuJjnb","zMHhMfsEPvKjgFuyE"],"teamId":"6072f51066b377a354d793cc"}
    jsonObj[QLatin1String("rooms")] = QJsonArray::fromStringList(mRoomIds);
    jsonObj[QLatin1String("teamId")] = mTeamId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
