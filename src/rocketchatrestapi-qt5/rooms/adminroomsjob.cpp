/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adminroomsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
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

    addStartRestApiInfo(QByteArrayLiteral("RoomsAdminJob: Ask info about room admin info"));
    return true;
}

void AdminRoomsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("RoomsAdminJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT adminRoomsDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("RoomsAdminJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
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
        urlQuery.addQueryItem(QStringLiteral("filter"), mRoomsAdminInfo.filter);
    }
    QStringList types;
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Direct) {
        types << QStringLiteral("d");
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Private) {
        types << QStringLiteral("p");
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Channel) {
        types << QStringLiteral("c");
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::Team) {
        types << QStringLiteral("teams");
    }
    if (mRoomsAdminInfo.searchType & AdminRoomSearchType::OmniChannel) {
        types << QStringLiteral("l");
    }
    if (!types.isEmpty()) {
        for (const QString &str : std::as_const(types)) {
            urlQuery.addQueryItem(QStringLiteral("types[]"), str);
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
