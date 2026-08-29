/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "setroomkeyidjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
SetRoomKeyIDJob::SetRoomKeyIDJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

SetRoomKeyIDJob::~SetRoomKeyIDJob() = default;

bool SetRoomKeyIDJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("SetRoomKeyIDJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void SetRoomKeyIDJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("SetRoomKeyIDJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT setRoomKeyIdDone();
    } else if (replyObject["errorType"_L1].toString() == "error-room-e2e-key-already-exists"_L1) {
        // Another client won the room key race. Let callers recover using the server key id.
        addLoggerInfo("SetRoomKeyIDJob: room key already exists, recovery path will be used"_ba);
        Q_EMIT roomKeyIdAlreadyExists();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("SetRoomKeyIDJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

SetRoomKeyIDJob::RoomKeyIDInfo SetRoomKeyIDJob::roomKeyIDInfo() const
{
    return mSetRoomKeyIDInfo;
}

void SetRoomKeyIDJob::setRoomKeyIDInfo(const RoomKeyIDInfo &newSetRoomKeyIDInfo)
{
    mSetRoomKeyIDInfo = newSetRoomKeyIDInfo;
}

bool SetRoomKeyIDJob::requireHttpAuthentication() const
{
    return true;
}

bool SetRoomKeyIDJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mSetRoomKeyIDInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "SetRoomKeyIDJob: mSetRoomKeyIDInfo is invalid";
        return false;
    }
    return true;
}

QNetworkRequest SetRoomKeyIDJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::E2ESetRoomKeyID);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument SetRoomKeyIDJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["rid"_L1] = QString::fromLatin1(mSetRoomKeyIDInfo.roomId);
    jsonObj["keyID"_L1] = QString::fromLatin1(mSetRoomKeyIDInfo.keyId);

    qDebug() << " SetRoomKeyIDJob::json " << jsonObj;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

bool SetRoomKeyIDJob::RoomKeyIDInfo::isValid() const
{
    return !roomId.isEmpty() && !keyId.isEmpty();
}

#include "moc_setroomkeyidjob.cpp"
