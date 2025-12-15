/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
AdminRoomsJob::AdminRoomsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

AdminRoomsJob::~AdminRoomsJob() = default;

bool AdminRoomsJob::requireHttpAuthentication() const
{
    return true;
}

bool AdminRoomsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start roomAdmin job";
        deleteLater();
        return false;
    }
    submitGetRequest();

    addStartRestApiInfo("RoomsAdminJob: Ask info about room admin info"_ba);
    return true;
}

void AdminRoomsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("RoomsAdminJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT adminRoomsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("RoomsAdminJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

AdminRoomsJob::AdminRoomsJobInfo AdminRoomsJob::roomsAdminInfo() const
{
    return mRoomsAdminInfo;
}

void AdminRoomsJob::setRoomsAdminInfo(const AdminRoomsJobInfo &roomsAdminInfo)
{
    mRoomsAdminInfo = roomsAdminInfo;
}

bool AdminRoomsJob::hasQueryParameterSupport() const
{
    return true;
}

void AdminRoomsJob::initialUrlParameters(QUrlQuery &urlQuery) const
{
    // https://<server>/api/v1/rooms.adminRooms?filter=&types[]=d,p,c,teams&sort={"name":1}&count=25&offset=25
    if (!mRoomsAdminInfo.filter.isEmpty()) {
        urlQuery.addQueryItem(u"filter"_s, mRoomsAdminInfo.filter);
    }
    QStringList types;
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Direct) {
        types << u"d"_s;
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Private) {
        types << u"p"_s;
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Channel) {
        types << u"c"_s;
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Team) {
        types << u"teams"_s;
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::OmniChannel) {
        types << u"l"_s;
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Discussion) {
        types << u"discussions"_s;
    }
    if (!types.isEmpty()) {
        for (const QString &str : std::as_const(types)) {
            urlQuery.addQueryItem(u"types[]"_s, str);
        }
    }
}

QNetworkRequest AdminRoomsJob::request() const
{
    // rooms.adminRooms?filter=&types[]=d,p,c,teams&sort={"name":1}&count=25
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RoomsAdminRooms);
    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    initialUrlParameters(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_adminroomsjob.cpp"
