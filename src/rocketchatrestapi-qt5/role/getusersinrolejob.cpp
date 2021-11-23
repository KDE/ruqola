/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &GetUsersInRoleJob::slotGetUsersInRoleFinished);
    addStartRestApiInfo(QByteArrayLiteral("GetUsersInRoleJob: Ask info about users in role"));
    return true;
}

void GetUsersInRoleJob::slotGetUsersInRoleFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetUsersInRoleJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT getUsersInRoleDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetUsersInRoleJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
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
