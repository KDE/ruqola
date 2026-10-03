/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channeldeletejob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

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
    addStartRestApiInfo("ChannelDeleteJob::start"_ba);
    submitPostRequest(json());

    return true;
}

void ChannelDeleteJob::onPostRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    if (checkResponse("ChannelDeleteJob"_ba, replyErrorString, replyJson)) {
        Q_EMIT deletechannelDone(channelGroupInfo().identifier);
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
