/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsbanuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsBanUserJob::RoomsBanUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsBanUserJob::~RoomsBanUserJob() = default;

bool RoomsBanUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsBanUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsBanUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsBanUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsBanUserDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsBanUserJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString RoomsBanUserJob::userName() const
{
    return mUserName;
}

void RoomsBanUserJob::setUserName(const QString &newUserName)
{
    mUserName = newUserName;
}

QByteArray RoomsBanUserJob::roomId() const
{
    return mRoomId;
}

void RoomsBanUserJob::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

bool RoomsBanUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsBanUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mUserName.isEmpty() || mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsBanUserJob: mUserName or mRoomId is empty.";
        return false;
    }

    return true;
}

QJsonDocument RoomsBanUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QString::fromLatin1(mRoomId);
    jsonObj["username"_L1] = mUserName;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest RoomsBanUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsBanUser);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_roomsbanuserjob.cpp"
