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

#include "channeljoinjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelJoinJob::ChannelJoinJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelJoinJob::~ChannelJoinJob()
{
}

bool ChannelJoinJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChannelJoinJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChannelJoinJob::slotChannelJoinFinished);
    return true;
}

void ChannelJoinJob::slotChannelJoinFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelJoinJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setChannelJoinDone(mRoomId);
        } else {
            addLoggerWarning(QByteArrayLiteral("ChannelJoinJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
            //Invalid password
            if (replyObject[QStringLiteral("errorType")].toString() == QLatin1String("error-code-invalid")) {
                Q_EMIT missingChannelPassword(mRoomId);
            }
        }
    }
    deleteLater();
}

QString ChannelJoinJob::joinCode() const
{
    return mJoinCode;
}

void ChannelJoinJob::setJoinCode(const QString &joinCode)
{
    mJoinCode = joinCode;
}

bool ChannelJoinJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelJoinJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelJoinJob job";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelJoinJob: RoomId is empty";
        return false;
    }
    return true;
}

QJsonDocument ChannelJoinJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    if (!mJoinCode.isEmpty()) {
        jsonObj[QLatin1String("joinCode")] = mJoinCode;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelJoinJob::roomId() const
{
    return mRoomId;
}

void ChannelJoinJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelJoinJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsJoin);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}
