/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelsconverttoteamjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
using namespace RocketChatRestApi;
ChannelsConvertToTeamJob::ChannelsConvertToTeamJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelsConvertToTeamJob::~ChannelsConvertToTeamJob() = default;

bool ChannelsConvertToTeamJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelsConvertToTeamJob::start");
    QNetworkReply *reply = submitPostRequest(json());
    connect(reply, &QNetworkReply::finished, this, &ChannelsConvertToTeamJob::slotChannelConvertToTeamFinished);
    return true;
}

void ChannelsConvertToTeamJob::slotChannelConvertToTeamFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());
    if (reply) {
        const QJsonDocument replyJson = convertToJsonDocument(reply);
        const QJsonObject replyObject = replyJson.object();

        if (replyObject[QStringLiteral("success")].toBool()) {
            addLoggerInfo(QByteArrayLiteral("ChannelsConvertToTeamJob success: ") + replyJson.toJson(QJsonDocument::Indented));
            Q_EMIT channelConvertToTeamDone(replyObject);
        } else {
            emitFailedMessage(replyObject, reply);
            addLoggerWarning(QByteArrayLiteral("ChannelsConvertToTeamJob: Problem: ") + replyJson.toJson(QJsonDocument::Indented));
        }
        reply->deleteLater();
    }
    deleteLater();
}

QString ChannelsConvertToTeamJob::channelId() const
{
    return mChannelId;
}

void ChannelsConvertToTeamJob::setChannelId(const QString &channelId)
{
    mChannelId = channelId;
}

bool ChannelsConvertToTeamJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelsConvertToTeamJob::canStart() const
{
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    if (mChannelId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelsConvertToTeamJob: mTeamId is empty";
        return false;
    }
    return true;
}

QNetworkRequest ChannelsConvertToTeamJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsConvertToTeam);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QJsonDocument ChannelsConvertToTeamJob::json() const
{
    QJsonObject jsonObj;
    jsonObj[QLatin1String("channelId")] = mChannelId;
    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}
