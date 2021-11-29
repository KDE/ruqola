/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelmembersjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelMembersJob::ChannelMembersJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelMembersJob::~ChannelMembersJob() = default;

bool ChannelMembersJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelMembersJob::start: ");
    QNetworkReply *reply = submitGetRequest();
    connect(reply, &QNetworkReply::finished, this, &ChannelMembersJob::slotChannelMembersFinished);
    return true;
}

void ChannelMembersJob::slotChannelMembersFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("channelMembersDone success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelMembersDone(replyObject, channelGroupInfo());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("channelMembersDone problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChannelMembersJob::generateErrorMessage(const QString &errorStr) const
{
    if (errorStr == QLatin1String("unauthorized")) {
        return i18n("Get Members not authorized in this room");
    }
    return ChannelGroupBaseJob::generateErrorMessage(errorStr);
}

ChannelMembersJob::ChannelType ChannelMembersJob::channelType() const
{
    return mChannelType;
}

void ChannelMembersJob::setChannelType(ChannelType channelType)
{
    mChannelType = channelType;
}

bool ChannelMembersJob::hasQueryParameterSupport() const
{
    return true;
}

bool ChannelMembersJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelMembersJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: RoomId and RoomName are empty";
        return false;
    }
    if (mChannelType == ChannelMembersJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest ChannelMembersJob::request() const
{
    QUrl url;
    switch (mChannelType) {
    case Channel:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsMembers);
        break;
    case Groups:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsMembers);
        break;
    case Direct:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImMembers);
        break;
    case Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelMembersJob: Type is not defined";
        break;
    }
    addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}
