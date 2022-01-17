/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsemessageurlutils.h"
#include <QUrl>
#include <QUrlQuery>

ParseMessageUrlUtils::ParseMessageUrlUtils()
{
}

ParseMessageUrlUtils::~ParseMessageUrlUtils()
{
}

bool ParseMessageUrlUtils::parseUrl(const QString &messageUrl)
{
    if (messageUrl.isEmpty()) {
        return false;
    }
    const QUrl url(messageUrl);
    const QUrlQuery query(url);
    const QList<QPair<QString, QString>> queryItems = query.queryItems();
    if (messageUrl.startsWith(QStringLiteral("https://go.rocket.chat/"))) {
        // qDebug() << "queryItems " << queryItems;

        mServerHost = query.queryItemValue(QStringLiteral("host"));
        mRoomId = query.queryItemValue(QStringLiteral("rid"));
        mMessageId = query.queryItemValue(QStringLiteral("mid"));
        mPath = query.queryItemValue(QStringLiteral("path"), QUrl::FullyDecoded);
        return true;
    } else {
        // Example https://<server name>/channel/python?msg=sn3gEQom7NcLxTg5h
        mMessageId = query.queryItemValue(QStringLiteral("msg"));
        if (mMessageId.isEmpty()) {
            return false;
        }
        mServerHost = url.host();
        mPath = url.path(QUrl::FullyDecoded);
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

QDebug operator<<(QDebug d, const ParseMessageUrlUtils &t)
{
    d << "mServerPath " << t.serverHost();
    d << "mRoomId " << t.roomId();
    d << "mMessageId " << t.messageId();
    d << "mPath " << t.path();
    d << "roomIdType " << static_cast<int>(t.roomIdType());
    return d;
}
