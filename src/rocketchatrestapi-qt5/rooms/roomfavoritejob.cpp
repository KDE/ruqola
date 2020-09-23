/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "roomfavoritejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
RoomFavoriteJob::RoomFavoriteJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomFavoriteJob::~RoomFavoriteJob()
{
}

bool RoomFavoriteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomFavoriteJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &RoomFavoriteJob::slotChangeFavoriteFinished);
    return true;
}

void RoomFavoriteJob::slotChangeFavoriteFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("RoomFavoriteJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeFavoriteDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("RoomFavoriteJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool RoomFavoriteJob::favorite() const
{
    return mFavorite;
}

void RoomFavoriteJob::setFavorite(bool favorite)
{
    mFavorite = favorite;
}

QString RoomFavoriteJob::roomId() const
{
    return mRoomId;
}

void RoomFavoriteJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool RoomFavoriteJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomFavoriteJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomFavoriteJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest RoomFavoriteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsFavorite);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument RoomFavoriteJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    jsonObj[QLatin1String("favorite")] = mFavorite;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
