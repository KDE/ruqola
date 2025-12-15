/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelkickjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

using namespace RocketChatRestApi;
using namespace Qt::Literals::StringLiterals;
ChannelKickJob::ChannelKickJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelKickJob::~ChannelKickJob() = default;

bool ChannelKickJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelKickJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelKickJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelKickJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT kickUserDone(replyObject);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelKickJob Problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChannelKickJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelKickJob::canStart() const
{
    if (mKickUserId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelKickJob: mKickUserId is empty";
        return false;
    }
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelKickJob: RoomId and roomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelKickJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);
    jsonObj["userId"_L1] = QLatin1StringView(kickUserId());

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelKickJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsKick);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QByteArray ChannelKickJob::kickUserId() const
{
    return mKickUserId;
}

void ChannelKickJob::setKickUserId(const QByteArray &kickUserId)
{
    mKickUserId = kickUserId;
}

#include "moc_channelkickjob.cpp"
