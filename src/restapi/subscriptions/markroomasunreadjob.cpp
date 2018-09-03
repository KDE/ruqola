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

#include "markroomasunreadjob.h"
#include "restapimethod.h"
#include "restapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
MarkRoomAsUnReadJob::MarkRoomAsUnReadJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

MarkRoomAsUnReadJob::~MarkRoomAsUnReadJob()
{
}

bool MarkRoomAsUnReadJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("MarkRoomAsUnReadJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &MarkRoomAsUnReadJob::slotMarkAsUnRead);
    return true;
}

void MarkRoomAsUnReadJob::slotMarkAsUnRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("MarkRoomAsUnReadJob: finished: ") + data);
            Q_EMIT markAsUnReadDone();
        } else {
            qCWarning(RESTAPI_LOG) <<" Problem when we tried to mark as unread" << data;
        }
    }
    deleteLater();
}

QString MarkRoomAsUnReadJob::objectId() const
{
    return mObjectId;
}

void MarkRoomAsUnReadJob::setObjectId(const QString &objectId)
{
    mObjectId = objectId;
}

MarkRoomAsUnReadJob::MarkAsUnReadObject MarkRoomAsUnReadJob::unReadObject() const
{
    return mUnReadObject;
}

void MarkRoomAsUnReadJob::setUnReadObject(MarkAsUnReadObject type)
{
    mUnReadObject = type;
}

bool MarkRoomAsUnReadJob::requireHttpAuthentication() const
{
    return true;
}

bool MarkRoomAsUnReadJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(RESTAPI_LOG) << "Impossible to start MarkRoomAsUnReadJob";
        return false;
    }
    if (mUnReadObject == Unknown) {
        qCWarning(RESTAPI_LOG) << "MarkRoomAsUnReadJob: object type undefined";
        return false;
    }

    if (mObjectId.isEmpty()) {
        qCWarning(RESTAPI_LOG) << "MarkRoomAsUnReadJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest MarkRoomAsUnReadJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::SubscriptionsUnRead);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, true);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    return request;
}

QJsonDocument MarkRoomAsUnReadJob::json() const
{
    QJsonObject jsonObj;
    if (mUnReadObject == Room) {
        jsonObj[QLatin1String("roomId")] = mObjectId;
    } else if (mUnReadObject == FromMessage) {
        QJsonObject fromMessage;
        fromMessage[QLatin1String("_id")] = mObjectId;
        jsonObj[QLatin1String("firstUnreadMessage")] = fromMessage;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
