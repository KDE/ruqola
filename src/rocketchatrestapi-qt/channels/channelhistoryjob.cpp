/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelhistoryjob.h"

#include "restapimethod.h"
#include "rocketchatqtrestapi_debug.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
using namespace RocketChatRestApi;
ChannelHistoryJob::ChannelHistoryJob(QObject *parent)
    : ChannelGroupBaseJob(parent)
{
}

ChannelHistoryJob::~ChannelHistoryJob() = default;

bool ChannelHistoryJob::start()
{
    if (!canStart()) {
        deleteLater();
        return false;
    }
    addStartRestApiInfo("ChannelHistoryJob::start");
    submitGetRequest();

    return true;
}

void ChannelHistoryJob::onGetRequestResponse(const QString &replyErrorString, const QJsonDocument &replyJson)
{
    const QJsonObject replyObject = replyJson.object();

    if (replyObject["success"_L1].toBool()) {
        addLoggerInfo("ChannelHistoryJob success: "_ba + replyJson.toJson(QJsonDocument::Indented));
        Q_EMIT channelHistoryDone(replyObject, channelGroupInfo());
    } else {
        emitFailedMessage(replyErrorString, replyObject);
        addLoggerWarning("ChannelHistoryJob problem: "_ba + replyJson.toJson(QJsonDocument::Indented));
    }
}

ChannelHistoryJob::ChannelHistoryInfo ChannelHistoryJob::channelHistoryInfo() const
{
    return mChannelHistoryInfo;
}

void ChannelHistoryJob::setChannelHistoryInfo(const ChannelHistoryInfo &channelHistoryInfo)
{
    mChannelHistoryInfo = channelHistoryInfo;
}

bool ChannelHistoryJob::requireHttpAuthentication() const
{
    return true;
}

bool ChannelHistoryJob::canStart() const
{
    //    if (!hasIdentifier()) {
    //        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: RoomId and RoomName are empty";
    //        return false;
    //    }
    if (mChannelHistoryInfo.channelType == ChannelHistoryJob::ChannelType::Unknown) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: Channel type is unknown.";
        return false;
    }
    if (mChannelHistoryInfo.roomId.isEmpty()) {
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: roomId is empty";
        return false;
    }
    if (!RestApiAbstractJob::canStart()) {
        return false;
    }
    return true;
}

QNetworkRequest ChannelHistoryJob::request() const
{
    QUrl url;
    switch (mChannelHistoryInfo.channelType) {
    case ChannelType::Channel:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ChannelsHistory);
        break;
    case ChannelType::Groups:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::GroupsHistory);
        break;
    case ChannelType::Direct:
        url = mRestApiMethod->generateUrl(RestApiUtil::RestApiUrlType::ImHistory);
        break;
    case ChannelType::Unknown:
        qCWarning(ROCKETCHATQTRESTAPI_LOG) << "ChannelHistoryJob: Type is not defined";
        break;
    }

    QUrlQuery queryUrl;
    if (!mChannelHistoryInfo.roomId.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("roomId"), QLatin1StringView(mChannelHistoryInfo.roomId));
    }
    if (!mChannelHistoryInfo.latestMessage.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("latest"), mChannelHistoryInfo.latestMessage);
    }
    if (!mChannelHistoryInfo.oldestMessage.isEmpty()) {
        queryUrl.addQueryItem(QStringLiteral("oldest"), mChannelHistoryInfo.oldestMessage);
    }
    if (mChannelHistoryInfo.offset > 0) {
        queryUrl.addQueryItem(QStringLiteral("offset"), QString::number(mChannelHistoryInfo.offset));
    }
    if (mChannelHistoryInfo.count > 0) {
        queryUrl.addQueryItem(QStringLiteral("count"), QString::number(mChannelHistoryInfo.count));
    }
    queryUrl.addQueryItem(QStringLiteral("inclusive"), mChannelHistoryInfo.inclusive ? QStringLiteral("true") : QStringLiteral("false"));
    queryUrl.addQueryItem(QStringLiteral("unreads"), mChannelHistoryInfo.unreads ? QStringLiteral("true") : QStringLiteral("false"));
    addQueryParameter(queryUrl);
    url.setQuery(queryUrl);

    QNetworkRequest request(url);
    addAuthRawHeader(request);
    addRequestAttribute(request);
    return request;
}

QDebug operator<<(QDebug d, const RocketChatRestApi::ChannelHistoryJob::ChannelHistoryInfo &t)
{
    d.space() << "latestMessage " << t.latestMessage;
    d.space() << "oldestMessage " << t.oldestMessage;
    d.space() << "channelType " << t.channelType;
    d.space() << "offset " << t.offset;
    d.space() << "count " << t.count;
    d.space() << "inclusive " << t.inclusive;
    d.space() << "unreads " << t.unreads;
    d.space() << "roomId " << t.roomId;
    return d;
}

#include "moc_channelhistoryjob.cpp"
