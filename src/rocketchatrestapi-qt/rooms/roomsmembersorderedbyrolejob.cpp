/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomsmembersorderedbyrolejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

RoomsMembersOrderedByRoleJob::~RoomsMembersOrderedByRoleJob() = default;

bool RoomsMembersOrderedByRoleJob::requireHttpAuthentication() const
{
    return true;
}

bool RoomsMembersOrderedByRoleJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (!mRoomsMembersOrderedByRoleJobInfo.isValid()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "RoomsMembersOrderedByRoleJob: mRoomsMembersOrderedByRoleJobInfo is empty";
        return false;
    }
    return true;
}

bool RoomsMembersOrderedByRoleJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start RoomsMembersOrderedByRoleJob job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo(QByteArrayLiteral("RoomsMembersOrderedByRoleJob: Ask discussions in room"));
    return true;
}

void RoomsMembersOrderedByRoleJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomsMembersOrderedByRoleJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsImagesDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomsMembersOrderedByRoleJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo RoomsMembersOrderedByRoleJob::roomsImagesJobInfo() const
{
    return mRoomsMembersOrderedByRoleJobInfo;
}

void RoomsMembersOrderedByRoleJob::setRoomsMembersOrderedByRoleJobInfo(const RoomsMembersOrderedByRoleJobInfo &newRoomsMembersOrderedByRoleJobInfo)
{
    mRoomsMembersOrderedByRoleJobInfo = newRoomsMembersOrderedByRoleJobInfo;
}

bool RoomsMembersOrderedByRoleJob::hasQueryParameterSupport() const
{
    return true;
}

QNetworkRequest RoomsMembersOrderedByRoleJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsImages);
    QUrlQuery queryUrl;
    mRoomsMembersOrderedByRoleJobInfo.generateRequest(queryUrl);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request);

    addAuthRawHeader(request);
    return request;
}

bool RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo::isValid() const
{
    return !roomId.isEmpty() && !startingFromId.isEmpty() && (count > 0);
}

void RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo::generateRequest(QUrlQuery &query) const
{
    query.addQueryItem(QStringLiteral("roomId"), QLatin1StringView(roomId));
    query.addQueryItem(QStringLiteral("startingFromId"), QLatin1StringView(startingFromId));
    query.addQueryItem(QStringLiteral("offset"), QString::number(offset));
    query.addQueryItem(QStringLiteral("count"), QString::number(count));
}

#include "moc_roomsmembersorderedbyrolejob.cpp"
