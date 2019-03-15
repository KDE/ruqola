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

#include "channelremovemoderatorjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelRemoveModeratorJob::ChannelRemoveModeratorJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelRemoveModeratorJob::~ChannelRemoveModeratorJob()
{
}

bool ChannelRemoveModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("ChannelRemoveModeratorJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &ChannelRemoveModeratorJob::slotRemoveModeratorFinished);
    return true;
}

void ChannelRemoveModeratorJob::slotRemoveModeratorFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelRemoveModeratorJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeModeratorDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("ChannelRemoveModeratorJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChannelRemoveModeratorJob::removeUserId() const
{
    return mRemoveUserId;
}

void ChannelRemoveModeratorJob::setRemoveUserId(const QString &removeUserId)
{
    mRemoveUserId = removeUserId;
}

bool ChannelRemoveModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelRemoveModeratorJob::canStart() const
{
    if (mRemoveUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveModeratorJob: remove userid is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelRemoveModeratorJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start ChannelRemoveModeratorJob job";
        return false;
    }
    return true;
}

QJsonDocument ChannelRemoveModeratorJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("userId")] = removeUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString ChannelRemoveModeratorJob::roomId() const
{
    return mRoomId;
}

void ChannelRemoveModeratorJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest ChannelRemoveModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
