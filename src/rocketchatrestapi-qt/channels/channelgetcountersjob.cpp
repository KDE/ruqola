/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelgetcountersjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ChannelGetCountersJob::ChannelGetCountersJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelGetCountersJob::~ChannelGetCountersJob() = default;

bool ChannelGetCountersJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelGetCountersJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool ChannelGetCountersJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    submitGetRequest();
    addStartRestApiInfo("ChannelGetCountersJob::start");

    return true;
}

QNetworkRequest ChannelGetCountersJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsCounters);
    addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);

    return request;
}

bool ChannelGetCountersJob::requireHttpAuthentication() const
{
    return true;
}

void ChannelGetCountersJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelGetCountersJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelGetCountersDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelGetCountersJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChannelGetCountersJob::hasQueryParameterSupport() const
{
    return false;
}

#include "moc_channelgetcountersjob.cpp"
