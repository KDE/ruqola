/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinfojob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ChannelInfoJob::ChannelInfoJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelInfoJob::~ChannelInfoJob() = default;

bool ChannelInfoJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelInfoJob::start: ");
    submitGetRequest();

    return true;
}

void ChannelInfoJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("channelInfoDone success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelInfoDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("channelInfoDone problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChannelInfoJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelInfoJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelInfoJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest ChannelInfoJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsInfo);
    addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

#include "moc_channelinfojob.cpp"
