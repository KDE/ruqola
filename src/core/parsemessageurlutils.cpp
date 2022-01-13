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
    if (messageUrl.startsWith(QStringLiteral("https://go.rocket.chat/"))) {
        QUrl url(messageUrl);
        QUrlQuery query(url);
        QList<QPair<QString, QString>> queryItems = query.queryItems();
        qDebug() << "queryItems " << queryItems;

        mServerHost = query.queryItemValue(QStringLiteral("host"));
        mRoomId = query.queryItemValue(QStringLiteral("rid"));
        mMessageId = query.queryItemValue(QStringLiteral("mid"));
        mPath = query.queryItemValue(QStringLiteral("path"), QUrl::FullyDecoded);
        qDebug() << " mPath " << mPath;
        return true;
    }
    // https://go.rocket.chat/room?rid=XQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH&mid=Bo8pcAH86LxiYzu98&host=<server
    // name>&path=direct%2FXQv6u7Kyb4pfDhS4wuKK39zoewTkdacidH%3Fmsg%3DBo8pcAH86LxiYzu98
    // https://go.rocket.chat/room?rid=NCrToCewka5MgMcDM&mid=Xope7b8WYWz82yHaq&host=<server name>&path=channel%2Fthales%3Fmsg%3DXope7b8WYWz82yHaq
    // TODO
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

QDebug operator<<(QDebug d, const ParseMessageUrlUtils &t)
{
    d << "mServerPath " << t.serverHost();
    d << "mRoomId " << t.roomId();
    d << "mMessageId " << t.messageId();
    d << "mPath " << t.path();
    return d;
}
