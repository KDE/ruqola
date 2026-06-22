/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsunbanuserjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
RoomsUnbanUserJob::RoomsUnbanUserJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsUnbanUserJob::~RoomsUnbanUserJob() = default;

bool RoomsUnbanUserJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("RoomsUnbanUserJob::start");
    submitPostRequest(json());

    return true;
}

void RoomsUnbanUserJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsUnbanUserJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsUnbanUserDone();
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsUnbanUserJob: problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

RoomsUnbanUserJob::RoomsUnbanUserInfo RoomsUnbanUserJob::roomsUnbanUserInfo() const
{
    return mRoomsUnbanUserInfo;
}

void RoomsUnbanUserJob::setRoomsUnbanUserInfo(const RoomsUnbanUserInfo &newRoomsUnbanUserInfo)
{
    mRoomsUnbanUserInfo = newRoomsUnbanUserInfo;
}

bool RoomsUnbanUserJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsUnbanUserJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoomsUnbanUserInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsUnbanUserJob: mUserName or mRoomId is empty.";
        return false;
    }

    return true;
}

QJsonDocument RoomsUnbanUserJob::json() const
{
    QJsonObject jsonObj;
    jsonObj["roomId"_L1] = QString::fromLatin1(mRoomsUnbanUserInfo.roomId);
    switch (mRoomsUnbanUserInfo.type) {
    case IdentifierType::Unknown:
        break;
    case IdentifierType::UserId:
        jsonObj["userId"_L1] = mRoomsUnbanUserInfo.identifier;
        break;
    case IdentifierType::UserName:
        jsonObj["username"_L1] = mRoomsUnbanUserInfo.identifier;
        break;
    }

    const QJsonDocument postData = QJsonDocument(jsonObj);
    // qDebug() << " postData**************** " << postData;
    return postData;
}

QNetworkRequest RoomsUnbanUserJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsUnbanUser);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool RoomsUnbanUserJob::RoomsUnbanUserInfo::isValid() const
{
    return type != IdentifierType::Unknown && !identifier.isEmpty() && !roomId.isEmpty();
}

#include "moc_roomsunbanuserjob.cpp"
