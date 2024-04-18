/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parserocketchaturlutils.h"
#include "ruqola_debug.h"
#include <QUrl>
#include <QUrlQuery>
using namespace Qt::Literals::StringLiterals;
// https://developer.rocket.chat/getting-started/deep-linking

ParseRocketChatUrlUtils::ParseRocketChatUrlUtils() = default;

ParseRocketChatUrlUtils::~ParseRocketChatUrlUtils() = default;

ParseRocketChatUrlUtils::UrlType ParseRocketChatUrlUtils::parseUrl(const QString &messageUrl)
{
    if (messageUrl.isEmpty()) {
        return ParseRocketChatUrlUtils::UrlType::Unknown;
    }
    QString newMessageUrl = messageUrl;
    newMessageUrl.replace("rocketchat://"_L1, "https://go.rocket.chat/"_L1);
    QUrl url(newMessageUrl);
    const QUrlQuery query(url);
    if (newMessageUrl.startsWith(QStringLiteral("https://go.rocket.chat/"))) {
        const QString urlPath{url.path()};
        qDebug() << url;
        if (urlPath == "/room"_L1) {
            // qDebug() << "queryItems " << queryItems;

            mParsingInfo.serverHost = query.queryItemValue(QStringLiteral("host"));
            mParsingInfo.roomId = query.queryItemValue(QStringLiteral("rid"));
            mParsingInfo.messageId = query.queryItemValue(QStringLiteral("mid"));
            mParsingInfo.path = query.queryItemValue(QStringLiteral("path"), QUrl::FullyDecoded);
            if (!mParsingInfo.path.isEmpty()) {
                mParsingInfo.roomIdType = RoomIdType::RoomId;
                if (mParsingInfo.path.startsWith(QStringLiteral("direct"))) {
                    mParsingInfo.channelType = ChannelType::Direct;
                } else if (mParsingInfo.path.startsWith(QStringLiteral("channel"))) {
                    mParsingInfo.channelType = ChannelType::Channel;
                } else if (mParsingInfo.path.startsWith(QStringLiteral("group"))) {
                    mParsingInfo.channelType = ChannelType::Group;
                } else {
                    qCWarning(RUQOLA_LOG) << "Unknown channel type " << mParsingInfo.path;
                    return ParseRocketChatUrlUtils::UrlType::Unknown;
                }
            } else {
                return ParseRocketChatUrlUtils::UrlType::Unknown;
            }
            return ParseRocketChatUrlUtils::UrlType::Message;
        } else if (urlPath == "/auth"_L1) {
            mParsingInfo.serverHost = query.queryItemValue(QStringLiteral("host"));
            mParsingInfo.token = query.queryItemValue(QStringLiteral("token"));
            mParsingInfo.userId = query.queryItemValue(QStringLiteral("userId")).toLatin1();
            return ParseRocketChatUrlUtils::UrlType::Server;
        } else if (urlPath == "/invite"_L1) {
            mParsingInfo.serverHost = query.queryItemValue(QStringLiteral("host"));
            // TODO
            return ParseRocketChatUrlUtils::UrlType::Invite;
        } else if (urlPath == "/conference"_L1) {
            mParsingInfo.serverHost = query.queryItemValue(QStringLiteral("host"));
            // TODO
            return ParseRocketChatUrlUtils::UrlType::ConferenceCall;
        } else {
            qCWarning(RUQOLA_LOG) << "Unknown path type " << urlPath;
            return ParseRocketChatUrlUtils::UrlType::Unknown;
        }
    } else {
        // Example https://<server url>/channel/python?msg=sn3gEQom7NcLxTg5h
        mParsingInfo.messageId = query.queryItemValue(QStringLiteral("msg"));
        mParsingInfo.serverHost = url.host();
        mParsingInfo.path = url.path(QUrl::FullyDecoded);
        mParsingInfo.roomIdType = RoomIdType::RoomName;
        url.setQuery(QUrlQuery());
        QString urlPathDecoded{url.path(QUrl::FullyDecoded)};
        if (urlPathDecoded.contains(QStringLiteral("/channel/"))) {
            mParsingInfo.roomId = urlPathDecoded.remove(QStringLiteral("/channel/"));
            mParsingInfo.channelType = ChannelType::Channel;
        } else if (urlPathDecoded.contains(QStringLiteral("/direct/"))) {
            mParsingInfo.roomId = urlPathDecoded.remove(QStringLiteral("/direct/"));
            mParsingInfo.channelType = ChannelType::Direct;
        } else if (urlPathDecoded.contains(QStringLiteral("/group/"))) {
            mParsingInfo.roomId = urlPathDecoded.remove(QStringLiteral("/group/"));
            mParsingInfo.channelType = ChannelType::Group;
        } else {
            mParsingInfo.serverHost.clear();
            mParsingInfo.path.clear();
            mParsingInfo.roomIdType = RoomIdType::Unknown;
            return ParseRocketChatUrlUtils::UrlType::Unknown;
        }
        return ParseRocketChatUrlUtils::UrlType::Message;
    }
    return ParseRocketChatUrlUtils::UrlType::Unknown;
}

ParseRocketChatUrlUtils::ParsingInfo ParseRocketChatUrlUtils::parsingInfo() const
{
    return mParsingInfo;
}

void ParseRocketChatUrlUtils::setParsingInfo(const ParsingInfo &newParsingInfo)
{
    mParsingInfo = newParsingInfo;
}

QDebug operator<<(QDebug d, const ParseRocketChatUrlUtils::ParsingInfo &t)
{
    d.space() << "mServerPath" << t.serverHost;
    d.space() << "mRoomId" << t.roomId;
    d.space() << "mMessageId" << t.messageId;
    d.space() << "mPath" << t.path;
    d.space() << "mRoomIdType" << t.roomIdType;
    d.space() << "mChannelType" << t.channelType;
    d.space() << "mToken" << t.token;
    d.space() << "mUserId" << t.userId;
    return d;
}

#include "moc_parserocketchaturlutils.cpp"
