/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsemessageurlutils.h"
#include "ruqola_debug.h"
#include <QUrl>
#include <QUrlQuery>

ParseMessageUrlUtils::ParseMessageUrlUtils() = default;

ParseMessageUrlUtils::~ParseMessageUrlUtils() = default;

bool ParseMessageUrlUtils::parseUrl(const QString &messageUrl)
{
    if (messageUrl.isEmpty()) {
        return false;
    }
    QUrl url(messageUrl);
    const QUrlQuery query(url);
    // const QList<QPair<QString, QString>> queryItems = query.queryItems();
    if (messageUrl.startsWith(QStringLiteral("https://go.rocket.chat/"))) {
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
            } else {
                qCWarning(RUQOLA_LOG) << "Unknown channel type " << mPath;
                return false;
            }
        } else {
            return false;
        }
        return true;
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
        } else {
            mServerHost.clear();
            mPath.clear();
            mRoomIdType = RoomIdType::Unknown;
            return false;
        }
        return true;
    }
    return false;
}

const QString &ParseMessageUrlUtils::messageId() const
{
    return mMessageId;
}

void ParseMessageUrlUtils::setMessageId(const QString &newMessageId)
{
    mMessageId = newMessageId;
}

const QString &ParseMessageUrlUtils::roomId() const
{
    return mRoomId;
}

void ParseMessageUrlUtils::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

const QString &ParseMessageUrlUtils::serverHost() const
{
    return mServerHost;
}

void ParseMessageUrlUtils::setServerHost(const QString &newServerPath)
{
    mServerHost = newServerPath;
}

const QString &ParseMessageUrlUtils::path() const
{
    return mPath;
}

void ParseMessageUrlUtils::setPath(const QString &newPath)
{
    mPath = newPath;
}

ParseMessageUrlUtils::RoomIdType ParseMessageUrlUtils::roomIdType() const
{
    return mRoomIdType;
}

void ParseMessageUrlUtils::setRoomIdType(ParseMessageUrlUtils::RoomIdType newRoomIdType)
{
    mRoomIdType = newRoomIdType;
}

ParseMessageUrlUtils::ChannelType ParseMessageUrlUtils::channelType() const
{
    return mChannelType;
}

void ParseMessageUrlUtils::setChannelType(ChannelType newChannelType)
{
    mChannelType = newChannelType;
}

QDebug operator<<(QDebug d, const ParseMessageUrlUtils &t)
{
    d << "mServerPath " << t.serverHost();
    d << "mRoomId " << t.roomId();
    d << "mMessageId " << t.messageId();
    d << "mPath " << t.path();
    d << "roomIdType " << t.roomIdType();
    d << "channelType " << t.channelType();
    return d;
}
