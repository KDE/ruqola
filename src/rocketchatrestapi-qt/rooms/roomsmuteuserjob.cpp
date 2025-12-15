/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmuteuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsMuteUserJob::RoomsMuteUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsMuteUserJob::~RoomsMuteUserJob() = default;

bool RoomsMuteUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsMuteUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsMuteUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsMuteUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsMuteUserDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsMuteUserJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RoomsMuteUserJob::userName() const
{
    return mUserName;
}

void RoomsMuteUserJob::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QByteArray RoomsMuteUserJob::roomId() const
{
    return mRoomId;
}

void RoomsMuteUserJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool RoomsMuteUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsMuteUserJob::canStart() const
{
    if (mUserName.isEmpty() || mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsMuteUserJob: mUserName or mRoomId is empty.";
        return false;
    }

    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument RoomsMuteUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QString::fromLatin1(mRoomId);
    jsonObj["username"_L1] = mUserName;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

QNetworkRequest RoomsMuteUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsMuteUser);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsmuteuserjob.cpp"
