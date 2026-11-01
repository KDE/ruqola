/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getroomhistoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
GetRoomHistoryJob::GetRoomHistoryJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetRoomHistoryJob::~GetRoomHistoryJob() = default;

bool GetRoomHistoryJob::requireHttpAuthentication() const
{
    return true;
}

bool GetRoomHistoryJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoomHistoryInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetRoomHistoryJob: RoomId is empty";
        return false;
    }
    return true;
}

bool GetRoomHistoryJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start GetRoomHistoryJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("GetRoomHistoryJob: Ask info about room history info"_ba);
    return true;
}

void GetRoomHistoryJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("GetRoomHistoryJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT getRoomHistoryDone(*replyObject);
    }
}

GetRoomHistoryJob::GetRoomHistoryInfo GetRoomHistoryJob::roomHistoryInfo() const
{
    return mRoomHistoryInfo;
}

void GetRoomHistoryJob::setRoomHistoryInfo(const GetRoomHistoryInfo &newRoomHistoryInfo)
{
    mRoomHistoryInfo = newRoomHistoryInfo;
}

QNetworkRequest GetRoomHistoryJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsHistory);
    mRoomHistoryInfo.addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

bool GetRoomHistoryJob::GetRoomHistoryInfo::isValid() const
{
    return !roomId.isEmpty();
}

void GetRoomHistoryJob::GetRoomHistoryInfo::addQueryItem(QUrl &url) const
{
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"roomId"_s, QLatin1StringView(roomId));
    if (count != 20) {
        queryUrl.addQueryItem(u"count"_s, QString::number(count));
    }
    queryUrl.addQueryItem(u"showThreadMessages"_s, showThreadMessages ? u"true"_s : u"false"_s);
    if (!aroundId.isEmpty()) {
        queryUrl.addQueryItem(u"aroundId"_s, QLatin1StringView(aroundId));
    }
    if (!lastSeen.isEmpty()) {
        queryUrl.addQueryItem(u"lastSeen"_s, lastSeen);
    }
    if (!previous.isEmpty()) {
        queryUrl.addQueryItem(u"previous"_s, previous);
    }
    if (!next.isEmpty()) {
        queryUrl.addQueryItem(u"next"_s, next);
    }
    url.setQuery(queryUrl);
}

#include "moc_getroomhistoryjob.cpp"
