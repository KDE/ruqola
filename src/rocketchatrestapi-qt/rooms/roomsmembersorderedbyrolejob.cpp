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

    addStartRestApiInfo("RoomsMembersOrderedByRoleJob: Ask discussions in room"_ba);
    return true;
}

void RoomsMembersOrderedByRoleJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsMembersOrderedByRoleJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT roomsMembersOrderedByRoleDone(replyObject, mRoomsMembersOrderedByRoleJobInfo.roomId, mRoomsMembersOrderedByRoleJobInfo.filter);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsMembersOrderedByRoleJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo RoomsMembersOrderedByRoleJob::roomsMembersOrderedByRoleJobInfo() const
{
    return mRoomsMembersOrderedByRoleJobInfo;
}

void RoomsMembersOrderedByRoleJob::setRoomsMembersOrderedByRoleJobInfo(const RoomsMembersOrderedByRoleJobInfo &newRoomsMembersOrderedByRoleJobInfo)
{
    mRoomsMembersOrderedByRoleJobInfo = newRoomsMembersOrderedByRoleJobInfo;
}

bool RoomsMembersOrderedByRoleJob::hasQueryParameterSupport() const
{
    return false;
}

QNetworkRequest RoomsMembersOrderedByRoleJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsMembersOrderedByRole);
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
    return !roomId.isEmpty() && (count > 0);
}

void RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo::generateRequest(QUrlQuery &query) const
{
    query.addQueryItem(QStringLiteral("roomId"), QLatin1StringView(roomId));
    query.addQueryItem(QStringLiteral("offset"), QString::number(offset));
    query.addQueryItem(QStringLiteral("count"), QString::number(count));
    if (!filter.isEmpty()) {
        query.addQueryItem(QStringLiteral("filter"), filter);
    }
}

QDebug operator<<(QDebug d, const RocketChatRestApi::RoomsMembersOrderedByRoleJob::RoomsMembersOrderedByRoleJobInfo &t)
{
    d.space() << "roomId:" << t.roomId;
    d.space() << "offset:" << t.offset;
    d.space() << "count:" << t.count;
    d.space() << "filter:" << t.filter;
    return d;
}

#include "moc_roomsmembersorderedbyrolejob.cpp"
