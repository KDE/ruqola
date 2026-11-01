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
    if (mRoomId.isEmpty()) {
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

    addStartRestApiInfo("GetRoomHistoryJob: Ask info about room admin info"_ba);
    return true;
}

void GetRoomHistoryJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (const auto replyObject = checkResponse("GetRoomHistoryJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT adminRoomGetRoomDone(*replyObject);
    }
}

const QString &GetRoomHistoryJob::roomId() const
{
    return mRoomId;
}

void GetRoomHistoryJob::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

QNetworkRequest GetRoomHistoryJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsAdminRoomsGetRoom);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(u"rid"_s, mRoomId);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_getroomhistoryjob.cpp"
