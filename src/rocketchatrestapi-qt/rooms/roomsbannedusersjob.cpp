/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsbannedusersjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsBannedUsersJob::RoomsBannedUsersJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsBannedUsersJob::~RoomsBannedUsersJob() = default;

bool RoomsBannedUsersJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsBannedUsersJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoomsBannedUsersJobInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsBannedUsersJob: mRoomsBannedUsersJobInfo is empty";
        return false;
    }
    return true;
}

bool RoomsBannedUsersJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsBannedUsersJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("RoomsBannedUsersJob: Ask discussions in room"_ba);
    return true;
}

void RoomsBannedUsersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsBannedUsersJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsBannedUsersDone(replyObject, mRoomsBannedUsersJobInfo.roomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsBannedUsersJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

RoomsBannedUsersJob::RoomsBannedUsersJobInfo RoomsBannedUsersJob::roomsBannedUsersJobInfo() const
{
    return mRoomsBannedUsersJobInfo;
}

void RoomsBannedUsersJob::setRoomsBannedUsersJobInfo(const RoomsBannedUsersJobInfo &newRoomsBannedUsersJobInfo)
{
    mRoomsBannedUsersJobInfo = newRoomsBannedUsersJobInfo;
}

bool RoomsBannedUsersJob::hasQueryParameterSupport() const
{
    return false;
}

QNetworkRequest RoomsBannedUsersJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsBannedUsers);
    QUrlQuery queryUrl;
    mRoomsBannedUsersJobInfo.generateRequest(queryUrl);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

bool RoomsBannedUsersJob::RoomsBannedUsersJobInfo::isValid() const
{
    return !roomId.isEmpty() && (count > 0);
}

void RoomsBannedUsersJob::RoomsBannedUsersJobInfo::generateRequest(QUrlQuery &query) const
{
    query.addQueryItem(u"roomId"_s, QLatin1StringView(roomId));
    query.addQueryItem(u"offset"_s, QString::number(offset));
    query.addQueryItem(u"count"_s, QString::number(count));
}

QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsBannedUsersJob::RoomsBannedUsersJobInfo &t)
{
    d.space() << "roomId:" << t.roomId;
    d.space() << "offset:" << t.offset;
    d.space() << "count:" << t.count;
    return d;
}

#include "moc_roomsbannedusersjob.cpp"
