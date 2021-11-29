/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelgetcountersjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
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

    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo("ChannelGetCountersJob::start");
    connect(reply, &QNetworkReply::finished, this, &ChannelGetCountersJob::slotChannelGetCountersFinished);

    return true;
}

QNetworkRequest ChannelGetCountersJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsCounters);
    addQueryItem(url);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool ChannelGetCountersJob::requireHttpAuthentication() const
{
    return true;
}

void ChannelGetCountersJob::slotChannelGetCountersFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelGetCountersJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelGetCountersDone(replyObject, channelGroupInfo());
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelGetCountersJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

bool ChannelGetCountersJob::hasQueryParameterSupport() const
{
    return false;
}
