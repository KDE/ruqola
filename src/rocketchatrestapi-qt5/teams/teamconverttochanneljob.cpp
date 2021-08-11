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

#include "teamconverttochanneljob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamConvertToChannelJob::TeamConvertToChannelJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamConvertToChannelJob::~TeamConvertToChannelJob()
{
}

bool TeamConvertToChannelJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamConvertToChannelJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TeamConvertToChannelJob::slotTeamLeaveRoomFinished);
    return true;
}

void TeamConvertToChannelJob::slotTeamLeaveRoomFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamConvertToChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT removeLeaveRoomDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamConvertToChannelJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString TeamConvertToChannelJob::teamId() const
{
    return mTeamId;
}

void TeamConvertToChannelJob::setTeamId(const QString &teamId)
{
    mTeamId = teamId;
}

QString TeamConvertToChannelJob::roomId() const
{
    return mRoomId;
}

void TeamConvertToChannelJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool TeamConvertToChannelJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamConvertToChannelJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamConvertToChannelJob: mRoomId is empty";
        return false;
    }
    if (mTeamId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamConvertToChannelJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamConvertToChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsLeave);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamConvertToChannelJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    jsonObj[QLatin1String("teamId")] = mTeamId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
