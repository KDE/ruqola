/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getusersinrolejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
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
    addStartRestApiInfo("GetUsersInRoleJob: Ask info about users in role"_ba);
    return true;
}

void GetUsersInRoleJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetUsersInRoleJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT getUsersInRoleDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetUsersInRoleJob: Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool GetUsersInRoleJob::hasQueryParameterSupport() const
{
    return true;
}

void GetUsersInRoleJob::initialUrlParameters(QUrlQuery &urlQuery) const
{
    urlQuery.addQueryItem(u"role"_s, mRoleId);
#if 0
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
    if (!types.isEmpty()) {
        for (const QString &str : std::as_const(types)) {
            urlQuery.addQueryItem(u"types[]"_s, str);
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
