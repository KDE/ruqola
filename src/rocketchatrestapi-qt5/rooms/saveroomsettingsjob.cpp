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

#include "saveroomsettingsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
SaveRoomSettingsJob::SaveRoomSettingsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SaveRoomSettingsJob::~SaveRoomSettingsJob()
{
}

bool SaveRoomSettingsJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SaveRoomSettingsJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &SaveRoomSettingsJob::slotChangeNotificationFinished);
    return true;
}

void SaveRoomSettingsJob::slotChangeNotificationFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("SaveRoomSettingsJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT changeNotificationDone();
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("SaveRoomSettingsJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

SaveRoomSettingsJob::SaveRoomSettingsInfo SaveRoomSettingsJob::saveRoomSettingsInfo() const
{
    return mSaveRoomSettingsInfo;
}

void SaveRoomSettingsJob::setSaveRoomSettingsInfo(const SaveRoomSettingsInfo &saveRoomSettingsInfo)
{
    mSaveRoomSettingsInfo = saveRoomSettingsInfo;
}

bool SaveRoomSettingsJob::requireHttpAuthentication() const
{
    return true;
}

bool SaveRoomSettingsJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mSaveRoomSettingsInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SaveRoomSettingsJob: mRoomId is empty";
        return false;
    }
    return true;
}

QNetworkRequest SaveRoomSettingsJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsSaveNotification);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SaveRoomSettingsJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("roomId")] = mSaveRoomSettingsInfo.roomId;
    const QJsonArray systemSettingsArray = QJsonArray::fromStringList(mSaveRoomSettingsInfo.systemMessages);
    jsonObj[QLatin1String("systemMessages")] = systemSettingsArray;
    jsonObj[QLatin1String("retentionEnabled")] = mSaveRoomSettingsInfo.retentionEnabled;
    jsonObj[QLatin1String("retentionExcludePinned")] = mSaveRoomSettingsInfo.retentionExcludePinned;
    jsonObj[QLatin1String("retentionFilesOnly")] = mSaveRoomSettingsInfo.retentionFilesOnly;
    jsonObj[QLatin1String("retentionIgnoreThreads")] = mSaveRoomSettingsInfo.retentionIgnoreThreads;
    jsonObj[QLatin1String("retentionOverrideGlobal")] = mSaveRoomSettingsInfo.retentionOverrideGlobal;
    jsonObj[QLatin1String("retentionMaxAge")] = mSaveRoomSettingsInfo.retentionMaxAge;

    jsonObj[QLatin1String("roomName")] = mSaveRoomSettingsInfo.roomName;
    jsonObj[QLatin1String("roomTopic")] = mSaveRoomSettingsInfo.roomTopic;
    jsonObj[QLatin1String("roomAnnouncement")] = mSaveRoomSettingsInfo.roomAnnouncement;
    jsonObj[QLatin1String("roomDescription")] = mSaveRoomSettingsInfo.roomDescription;

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SaveRoomSettingsJob::SaveRoomSettingsInfo::isValid() const
{
    return !roomId.isEmpty();
}
