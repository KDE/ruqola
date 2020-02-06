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

#include "leavegroupsjob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
LeaveGroupsJob::LeaveGroupsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

LeaveGroupsJob::~LeaveGroupsJob()
{
}

bool LeaveGroupsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    const QByteArray baPostData = json().toJson(QJsonDocument::Compact);
    addLoggerInfo("LeaveGroupsJob::start: " + baPostData);
    QNetworkReply *reply = mNetworkAccessManager->post(request(), baPostData);
    connect(reply, &QNetworkReply::finished, this, &LeaveGroupsJob::slotLeaveGroupsFinished);
    return true;
}

void LeaveGroupsJob::slotLeaveGroupsFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("LeaveGroupsJob: Success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT leaveGroupsDone();
        } else {
            emitFailedMessage(replyObject);
            addLoggerWarning(QByteArrayLiteral("LeaveGroupsJob: problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool LeaveGroupsJob::requireHttpAuthentication() const
{
    return true;
}

bool LeaveGroupsJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "LeaveGroupsJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start LeaveGroupsJob job";
        return false;
    }
    return true;
}

QJsonDocument LeaveGroupsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = roomId();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QString LeaveGroupsJob::roomId() const
{
    return mRoomId;
}

void LeaveGroupsJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QNetworkRequest LeaveGroupsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsLeave);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
