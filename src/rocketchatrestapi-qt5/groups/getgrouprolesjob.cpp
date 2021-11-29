/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getgrouprolesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
GetGroupRolesJob::GetGroupRolesJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

GetGroupRolesJob::~GetGroupRolesJob() = default;

bool GetGroupRolesJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetGroupRolesJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetGroupRolesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo("GetGroupRolesJob::start");
    connect(reply, &QNetworkReply::finished, this, &GetGroupRolesJob::slotGetGroupRolesFinished);

    return true;
}

QNetworkRequest GetGroupRolesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsRoles);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addRequestAttribute(request, false);

    addAuthRawHeader(request);
    return request;
}

bool GetGroupRolesJob::requireHttpAuthentication() const
{
    return true;
}

void GetGroupRolesJob::slotGetGroupRolesFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetGroupRolesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT groupRolesDone(replyObject, mRoomId);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetGroupRolesJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString GetGroupRolesJob::roomId() const
{
    return mRoomId;
}

void GetGroupRolesJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}
