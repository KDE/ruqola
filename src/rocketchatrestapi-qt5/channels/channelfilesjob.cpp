/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelfilesjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelFilesJob::ChannelFilesJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelFilesJob::~ChannelFilesJob() = default;

bool ChannelFilesJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelFilesJob::start: ");
    submitGetRequest();

    return true;
}

void ChannelFilesJob::onGetRequestResponse(const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("channelFilesDone success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelFilesDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyObject);
        addLoggerWarning(QByteArrayLiteral("channelFilesDone problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

ChannelFilesJob::ChannelType ChannelFilesJob::channelType() const
{
    return mChannelType;
}

void ChannelFilesJob::setChannelType(ChannelType channelType)
{
    mChannelType = channelType;
}

bool ChannelFilesJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelFilesJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelFilesJob: RoomId and RoomName are empty";
        return false;
    }
    if (mChannelType == ChannelFilesJob::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelFilesJob: Channel type is unknown.";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest ChannelFilesJob::request() const
{
    QUrl url;
    switch (mChannelType) {
    case Channel:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsFiles);
        break;
    case Groups:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsFiles);
        break;
    case Direct:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImFiles);
        break;
    case Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelFilesJob: Type is not defined";
        break;
    }
    addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);
    return request;
}

bool ChannelFilesJob::hasQueryParameterSupport() const
{
    return true;
}
