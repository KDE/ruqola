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

#include "setjoincodechanneljob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetJoinCodeChannelJob::SetJoinCodeChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetJoinCodeChannelJob::~SetJoinCodeChannelJob()
{
}

bool SetJoinCodeChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("SetJoinCodeChannelJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &SetJoinCodeChannelJob::slotSetJoinCodeFinished);
    return true;
}

void SetJoinCodeChannelJob::slotSetJoinCodeFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            qCDebug(ROCKETCHATQTRESTAPI_LOG) << "set join code success: " << data;
            Q_EMIT setJoinCodeDone();
        } else {
            qCWarning(ROCKETCHATQTRESTAPI_LOG) <<" Problem when we tried to assign code to channel" << data;
        }
    }
    deleteLater();
}

QString SetJoinCodeChannelJob::joinCode() const
{
    return mJoinCode;
}

void SetJoinCodeChannelJob::setJoinCode(const QString &joinCode)
{
    mJoinCode = joinCode;
}

bool SetJoinCodeChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool SetJoinCodeChannelJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start SetJoinCodeChannelJob job";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetJoinCodeChannelJob: RoomId is empty";
        return false;
    }
    if (mJoinCode.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetJoinCodeChannelJob: JoinCode is empty";
        return false;
    }
    return true;
}

QJsonDocument SetJoinCodeChannelJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("joinCode")] = joinCode();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString SetJoinCodeChannelJob::roomId() const
{
    return mRoomId;
}

void SetJoinCodeChannelJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest SetJoinCodeChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetJoinCode);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
