/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getchannelrolesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
GetChannelRolesJob::GetChannelRolesJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

GetChannelRolesJob::~GetChannelRolesJob() = default;

bool GetChannelRolesJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "GetChannelRolesJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool GetChannelRolesJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    submitGetRequest();
    addStartRestApiInfo("GetChannelRolesJob::start");
    return true;
}

QNetworkRequest GetChannelRolesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRoles);
    addQueryItem(url);
    QNetworkRequest request(url);
    addRequestAttribute(request);
    addAuthRawHeader(request);
    return request;
}

bool GetChannelRolesJob::requireHttpAuthentication() const
{
    return true;
}

void GetChannelRolesJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("GetChannelRolesJob: success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelRolesDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("GetChannelRolesJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString GetChannelRolesJob::jobName() const
{
    return i18n("Extract Roles");
}

#include "moc_getchannelrolesjob.cpp"
