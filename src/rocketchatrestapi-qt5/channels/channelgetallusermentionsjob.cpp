/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelgetallusermentionsjob.h"
#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>
using namespace RocketChatRestApi;
ChannelGetAllUserMentionsJob::ChannelGetAllUserMentionsJob(QObject *parent)
    : RestApiAbstractJob(parent)
{
}

ChannelGetAllUserMentionsJob::~ChannelGetAllUserMentionsJob() = default;

bool ChannelGetAllUserMentionsJob::canStart() const
{
    if (mRoomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelGetAllUserMentionsJob: RoomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

bool ChannelGetAllUserMentionsJob::start()
{
    if (!canStart()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "Impossible to start server info job";
        deleteLater();
        return false;
    }

    submitGetRequest();
    addStartRestApiInfo("ChannelGetAllUserMentionsJob::start");

    return true;
}

QNetworkRequest ChannelGetAllUserMentionsJob::request() const
{
    QUrl url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsGetAllUserMentionsByChannel);
    QUrlQuery queryUrl;
    queryUrl.addQueryItem(QStringLiteral("roomId"), mRoomId);
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);
    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request, false);

    return request;
}

bool ChannelGetAllUserMentionsJob::requireHttpAuthentication() const
{
    return true;
}

void ChannelGetAllUserMentionsJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();
    if (replyObject[QLatin1String("success")].toBool()) {
        addLoggerInfo(QByteArrayLiteral("ChannelGetAllUserMentionsJob success: ") + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelGetAllUserMentionsDone(replyObject, mRoomId);
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning(QByteArrayLiteral("ChannelGetAllUserMentionsJob problem: ") + replyJson.toJson(QJsonDocument::Indented));
    }
}

QString ChannelGetAllUserMentionsJob::roomId() const
{
    return mRoomId;
}

void ChannelGetAllUserMentionsJob::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

bool ChannelGetAllUserMentionsJob::hasQueryParameterSupport() const
{
    return true;
}

#include "moc_channelgetallusermentionsjob.cpp"
