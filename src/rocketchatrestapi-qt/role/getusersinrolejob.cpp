/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersinrolejob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetUsersInRoleJob::GetUsersInRoleJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetUsersInRoleJob::~GetUsersInRoleJob() = default;

bool GetUsersInRoleJob::requireHttpAuthentication() const
{
    return true;
}

bool GetUsersInRoleJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mRoleId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetUsersInRoleJob: mRoleId is empty.";
        return false;
    }
    return true;
}

bool GetUsersInRoleJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start roomAdmin job";
        deleteLater();
        return false;
    }
    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("GetUsersInRoleJob: Ask info about users in role"));
    return true;
}

void GetUsersInRoleJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("GetUsersInRoleJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getUsersInRoleDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("GetUsersInRoleJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GetUsersInRoleJob::hasQueryParameterSupport() const
{
    return true;
}

void GetUsersInRoleJob::initialUrlParameters(QUrlQuery &urlQuery) const
{
    urlQuery.addQueryItem(QStringLiteral("role"), mRoleId);
#if 0
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
    if (!types.isEmpty()) {
        for (const QString &str : std::as_const(types)) {
            urlQuery.addQueryItem(QStringLiteral("types[]"), str);
        }
    }
#endif
}

const QString &GetUsersInRoleJob::roleId() const
{
    return mRoleId;
}

void GetUsersInRoleJob::setRoleId(const QString &newRoleId)
{
    mRoleId = newRoleId;
}

QNetworkRequest GetUsersInRoleJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::RolesGetUsersInRole);
    QUrlQuery queryUrl;
    addQueryParameter(queryUrl);
    initialUrlParameters(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_getusersinrolejob.cpp"
