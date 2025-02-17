/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getgrouprolesjob.h"
using namespace Qt::Literals::StringLiterals;

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
GetGroupRolesJob::GetGroupRolesJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GetGroupRolesJob::~GetGroupRolesJob() = default;

bool GetGroupRolesJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetGroupRolesJob: RoomId and RoomName are empty";
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

    submitGetRequest();
    addStartRestApiInfo("GetGroupRolesJob::start");

    return true;
}

QNetworkRequest GetGroupRolesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsRoles);
    addQueryItem(url);
    QNetworkRequest request(url);
    addRequestAttribute(request);
    addAuthRawHeader(request);
    return request;
}

bool GetGroupRolesJob::requireHttpAuthentication() const
{
    return true;
}

QString GetGroupRolesJob::jobName() const
{
    return i18n("Extract Roles");
}

void GetGroupRolesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetGroupRolesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT groupRolesDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetGroupRolesJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

#include "moc_getgrouprolesjob.cpp"
