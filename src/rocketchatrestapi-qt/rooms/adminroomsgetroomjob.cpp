/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsgetroomjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
AdminRoomsGetRoomJob::AdminRoomsGetRoomJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AdminRoomsGetRoomJob::~AdminRoomsGetRoomJob() = default;

bool AdminRoomsGetRoomJob::requireHttpAuthentication() const
{
    return true;
}

bool AdminRoomsGetRoomJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "AdminRoomsGetRoomJob: RoomId is empty";
        return false;
    }
    return true;
}

bool AdminRoomsGetRoomJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start AdminRoomsGetRoomJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("AdminRoomsGetRoomJob: Ask info about room admin info"_ba);
    return true;
}

void AdminRoomsGetRoomJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("AdminRoomsGetRoomJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT adminRoomGetRoomDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("AdminRoomsGetRoomJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

const QString &AdminRoomsGetRoomJob::roomId() const
{
    return mRoomId;
}

void AdminRoomsGetRoomJob::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

QNetworkRequest AdminRoomsGetRoomJob::request() const
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

#include "moc_adminroomsgetroomjob.cpp"
