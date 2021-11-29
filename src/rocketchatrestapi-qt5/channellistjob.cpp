/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "channellistjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelListJob::ChannelListJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelListJob::~ChannelListJob() = default;

bool ChannelListJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start channel list job";
        deleteLater();
        return false;
    }

    QNetworkReply *reply = submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("ChannelListJob: ask channel list"));
    connect(reply, &QNetworkReply::finished, this, &ChannelListJob::slotListInfo);
    return false;
}

void ChannelListJob::slotListInfo()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();
        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelListJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelListDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelListJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
    }
    deleteLater();
}

QNetworkRequest ChannelListJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsList);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    return request;
}

bool ChannelListJob::requireHttpAuthentication() const
{
    return true;
}
