/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parserocketchaturlutils.h"
#include "ruqola_debug.h"
#include <QUrl>
#include <QUrlQuery>

// https://developer.rocket.chat/getting-started/deep-linking

ParseRocketChatUrlUtils::ParseRocketChatUrlUtils() = default;

ParseRocketChatUrlUtils::~ParseRocketChatUrlUtils() = default;

ParseRocketChatUrlUtils::UrlType ParseRocketChatUrlUtils::parseUrl(const QString &messageUrl)
{
    if (messageUrl.isEmpty()) {
        return ParseRocketChatUrlUtils::UrlType::Unknown;
    }
    QString newMessageUrl = messageUrl;
    newMessageUrl.replace(QLatin1String("rocketchat://"), QLatin1String("https://go.rocket.chat/"));
    QUrl url(newMessageUrl);
    const QUrlQuery query(url);
    if (newMessageUrl.startsWith(QStringLiteral("https://go.rocket.chat/")) || (url.scheme() == QLatin1String("rocketchat"))) {
        const QString urlPath{url.path()};
        qDebug() << url;
        if (urlPath == QLatin1String("/room")) {
            // qDebug() << "queryItems " << queryItems;

            mServerHost = query.queryItemValue(QStringLiteral("host"));
            mRoomId = query.queryItemValue(QStringLiteral("rid"));
            mMessageId = query.queryItemValue(QStringLiteral("mid"));
            mPath = query.queryItemValue(QStringLiteral("path"), QUrl::FullyDecoded);
            if (!mPath.isEmpty()) {
                mRoomIdType = RoomIdType::RoomId;
                if (mPath.startsWith(QStringLiteral("direct"))) {
                    mChannelType = ChannelType::Direct;
                } else if (mPath.startsWith(QStringLiteral("channel"))) {
                    mChannelType = ChannelType::Channel;
                } else if (mPath.startsWith(QStringLiteral("group"))) {
                    mChannelType = ChannelType::Group;
                } else {
                    qCWarning(RUQOLA_LOG) << "Unknown channel type " << mPath;
                    return ParseRocketChatUrlUtils::UrlType::Unknown;
                }
            } else {
                return ParseRocketChatUrlUtils::UrlType::Unknown;
            }
            return ParseRocketChatUrlUtils::UrlType::Message;
        } else if (urlPath == QLatin1String("/auth")) {
            mServerHost = query.queryItemValue(QStringLiteral("host"));
            // TODO
            return ParseRocketChatUrlUtils::UrlType::Server;
        } else if (urlPath == QLatin1String("/invite")) {
            mServerHost = query.queryItemValue(QStringLiteral("host"));
            // TODO
            return ParseRocketChatUrlUtils::UrlType::Invite;
        } else if (urlPath == QLatin1String("/conference")) {
            mServerHost = query.queryItemValue(QStringLiteral("host"));
            // TODO
            return ParseRocketChatUrlUtils::UrlType::ConferenceCall;
        } else {
            qCWarning(RUQOLA_LOG) << "Unknown path type " << urlPath;
            return ParseRocketChatUrlUtils::UrlType::Unknown;
        }
    } else {
        // Example https://<server url>/channel/python?msg=sn3gEQom7NcLxTg5h
        mMessageId = query.queryItemValue(QStringLiteral("msg"));
        mServerHost = url.host();
        mPath = url.path(QUrl::FullyDecoded);
        mRoomIdType = RoomIdType::RoomName;
        url.setQuery(QUrlQuery());
        QString urlPathDecoded{url.path(QUrl::FullyDecoded)};
        if (urlPathDecoded.contains(QStringLiteral("/channel/"))) {
            mRoomId = urlPathDecoded.remove(QStringLiteral("/channel/"));
            mChannelType = ChannelType::Channel;
        } else if (urlPathDecoded.contains(QStringLiteral("/direct/"))) {
            mRoomId = urlPathDecoded.remove(QStringLiteral("/direct/"));
            mChannelType = ChannelType::Direct;
        } else if (urlPathDecoded.contains(QStringLiteral("/group/"))) {
            mRoomId = urlPathDecoded.remove(QStringLiteral("/group/"));
            mChannelType = ChannelType::Group;
        } else {
            mServerHost.clear();
            mPath.clear();
            mRoomIdType = RoomIdType::Unknown;
            return ParseRocketChatUrlUtils::UrlType::Unknown;
        }
        return ParseRocketChatUrlUtils::UrlType::Message;
    }
    return ParseRocketChatUrlUtils::UrlType::Unknown;
}

const QString &ParseRocketChatUrlUtils::messageId() const
{
    return mMessageId;
}

void ParseRocketChatUrlUtils::setMessageId(const QString &newMessageId)
{
    mMessageId = newMessageId;
}

const QString &ParseRocketChatUrlUtils::roomId() const
{
    return mRoomId;
}

void ParseRocketChatUrlUtils::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

const QString &ParseRocketChatUrlUtils::serverHost() const
{
    return mServerHost;
}

void ParseRocketChatUrlUtils::setServerHost(const QString &newServerPath)
{
    mServerHost = newServerPath;
}

const QString &ParseRocketChatUrlUtils::path() const
{
    return mPath;
}

void ParseRocketChatUrlUtils::setPath(const QString &newPath)
{
    mPath = newPath;
}

ParseRocketChatUrlUtils::RoomIdType ParseRocketChatUrlUtils::roomIdType() const
{
    return mRoomIdType;
}

void ParseRocketChatUrlUtils::setRoomIdType(ParseRocketChatUrlUtils::RoomIdType newRoomIdType)
{
    mRoomIdType = newRoomIdType;
}

ParseRocketChatUrlUtils::ChannelType ParseRocketChatUrlUtils::channelType() const
{
    return mChannelType;
}

void ParseRocketChatUrlUtils::setChannelType(ChannelType newChannelType)
{
    mChannelType = newChannelType;
}

QString ParseRocketChatUrlUtils::token() const
{
    return mToken;
}

void ParseRocketChatUrlUtils::setToken(const QString &newToken)
{
    mToken = newToken;
}

QString ParseRocketChatUrlUtils::userId() const
{
    return mUserId;
}

void ParseRocketChatUrlUtils::setUserId(const QString &newUserId)
{
    mUserId = newUserId;
}

QDebug operator<<(QDebug d, const ParseRocketChatUrlUtils &t)
{
    d.space() << "mServerPath" << t.serverHost();
    d.space() << "mRoomId" << t.roomId();
    d.space() << "mMessageId" << t.messageId();
    d.space() << "mPath" << t.path();
    d.space() << "roomIdType" << t.roomIdType();
    d.space() << "channelType" << t.channelType();
    d.space() << "token" << t.token();
    d.space() << "userId" << t.userId();
    return d;
}

#include "moc_parserocketchaturlutils.cpp"
