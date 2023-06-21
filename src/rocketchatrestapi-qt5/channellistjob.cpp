/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    submitGetRequest();
    addStartRestApiInfo(QByteArrayLiteral("ChannelListJob: ask channel list"));
    return false;
}

void ChannelListJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QStringLiteral("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelListJob: success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelListDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelListJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
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

#include "moc_channellistjob.cpp"
