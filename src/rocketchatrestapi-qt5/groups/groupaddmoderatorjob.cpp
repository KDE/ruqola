/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "groupaddmoderatorjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
GroupAddModeratorJob::GroupAddModeratorJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GroupAddModeratorJob::~GroupAddModeratorJob()
{
}

bool GroupAddModeratorJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("GroupAddModeratorJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &GroupAddModeratorJob::slotAddModeratorFinished);
    return true;
}

void GroupAddModeratorJob::slotAddModeratorFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GroupAddModeratorJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT addModeratorDone();
        } else {
            addLoggerWarning(QByteArrayLiteral("GroupAddModeratorJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

QString GroupAddModeratorJob::addModeratorUserId() const
{
    return mAddModeratorUserId;
}

void GroupAddModeratorJob::setAddModeratorUserId(const QString &addModeratorUserId)
{
    mAddModeratorUserId = addModeratorUserId;
}

bool GroupAddModeratorJob::requireHttpAuthentication() const
{
    return true;
}

bool GroupAddModeratorJob::canStart() const
{
    if (mAddModeratorUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddModeratorJob: remove userid is empty";
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GroupAddModeratorJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GroupAddModeratorJob job";
        return false;
    }
    return true;
}

QJsonDocument GroupAddModeratorJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();
    jsonObj[QLatin1String("userId")] = addModeratorUserId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString GroupAddModeratorJob::roomId() const
{
    return mRoomId;
}

void GroupAddModeratorJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest GroupAddModeratorJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsAddModerator);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
