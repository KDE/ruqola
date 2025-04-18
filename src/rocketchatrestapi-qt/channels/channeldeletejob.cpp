/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeldeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ChannelDeleteJob::ChannelDeleteJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelDeleteJob::~ChannelDeleteJob() = default;

bool ChannelDeleteJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelDeleteJob::start");
    submitPostRequest(json());

    return true;
}

void ChannelDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelDeleteJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT deletechannelDone(channelGroupInfo().identifier);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelDeleteJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

bool ChannelDeleteJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelDeleteJob::canStart() const
{
    if (!hasIdentifier()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelDeleteJob: RoomId and RoomName are empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QJsonDocument ChannelDeleteJob::json() const
{
    QJsonObject jsonObj;
    generateJson(jsonObj);

    const QJsonDocument postData = QJsonDocument(jsonObj);
    return postData;
}

QNetworkRequest ChannelDeleteJob::request() const
{
    const QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsDelete);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

#include "moc_channeldeletejob.cpp"
