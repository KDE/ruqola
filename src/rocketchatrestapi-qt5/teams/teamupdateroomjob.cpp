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

#include "teamupdateroomjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
TeamUpdateRoomJob::TeamUpdateRoomJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

TeamUpdateRoomJob::~TeamUpdateRoomJob() = default;

bool TeamUpdateRoomJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("TeamUpdateRoomJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &TeamUpdateRoomJob::slotTeamUpdateRoomFinished);
    return true;
}

void TeamUpdateRoomJob::slotTeamUpdateRoomFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("TeamUpdateRoomJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT teamUpdateRoomDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("TeamUpdateRoomJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool TeamUpdateRoomJob::isDefault() const
{
    return mIsDefault;
}

void TeamUpdateRoomJob::setIsDefault(bool isDefault)
{
    mIsDefault = isDefault;
}

QString TeamUpdateRoomJob::roomId() const
{
    return mRoomId;
}

void TeamUpdateRoomJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool TeamUpdateRoomJob::requireHttpAuthentication() const
{
    return true;
}

bool TeamUpdateRoomJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "TeamUpdateRoomJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest TeamUpdateRoomJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::TeamsUpdateRoom);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument TeamUpdateRoomJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mRoomId;
    jsonObj[QLatin1String("isDefault")] = mIsDefault;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
