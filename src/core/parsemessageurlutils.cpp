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

void ParseMessageUrlUtils::parseUrl(const QString &url)
{
    // TODO
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
