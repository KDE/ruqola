/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "getchannelrolesjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <KLocalizedString>

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

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

    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo("GetChannelRolesJob::start");
    connect(reply, &QNetworkReply::finished, this, &GetChannelRolesJob::slotGetChannelRolesFinished);

    return true;
}

QNetworkRequest GetChannelRolesJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsRoles);
    addQueryItem(url);
    QNetworkRequest request(url);
    addRequestAttribute(request, false);
    addAuthRawHeader(request);
    return request;
}

bool GetChannelRolesJob::requireHttpAuthentication() const
{
    return true;
}

void GetChannelRolesJob::slotGetChannelRolesFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("GetChannelRolesJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelRolesDone(replyObject, channelGroupInfo());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("GetChannelRolesJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString GetChannelRolesJob::jobName() const
{
    return i18n("Extract Roles");
}
