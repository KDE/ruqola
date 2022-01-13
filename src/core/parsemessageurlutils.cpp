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

const QString &ParseMessageUrlUtils::serverPath() const
{
    return mServerPath;
}

void ParseMessageUrlUtils::setServerPath(const QString &newServerPath)
{
    mServerPath = newServerPath;
}

QDebug operator<<(QDebug d, const ParseMessageUrlUtils &t)
{
    d << "mServerPath " << t.serverPath();
    d << "mRoomId " << t.roomId();
    d << "mMessageId " << t.messageId();
    return d;
}
