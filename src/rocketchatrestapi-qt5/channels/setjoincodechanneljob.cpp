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

#include "setjoincodechanneljob.h"

#include "rocketchatqtrestapi_debug.h"
#include "restapimethod.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SetJoinCodeChannelJob::SetJoinCodeChannelJob(QObject *parent)
    : ChannelBaseJob(parent)
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
    addStartRestApiInfo("SetJoinCodeChannelJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SetJoinCodeChannelJob::slotSetJoinCodeFinished);
    return true;
}

void SetJoinCodeChannelJob::slotSetJoinCodeFinished()
{
    auto *reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QByteArray data = reply->readAll();
        const QJsonDocument replyJson = QJsonDocument::fromJson(data);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SetJoinCodeChannelJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT setJoinCodeDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SetJoinCodeChannelJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
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
        return false;
    }
    if (!hasRoomIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetJoinCodeChannelJob: RoomId and RoomName are empty";
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
    generateJson(jsonObj);
    jsonObj[QLatin1String("joinCode")] = joinCode();

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest SetJoinCodeChannelJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetJoinCode);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}
