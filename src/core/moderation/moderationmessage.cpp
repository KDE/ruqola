/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessage.h"
#include <QJsonObject>
ModerationMessage::ModerationMessage() = default;

ModerationMessage::~ModerationMessage() = default;

QString ModerationMessage::moderationId() const
{
    return mModerationId;
}

void ModerationMessage::setModerationId(const QString &newModerationId)
{
    mModerationId = newModerationId;
}

void ModerationMessage::parse(const QJsonObject &obj)
{
    mModerationId = obj[QLatin1StringView("_id")].toString();
    // TODO add timestamp
    const QJsonObject messageObject = obj[QLatin1StringView("message")].toObject();
    parseRoom(obj[QLatin1StringView("room")].toObject());
}

void ModerationMessage::parseRoom(const QJsonObject &roomObj)
{
    mRoomId = roomObj[QLatin1StringView("_id")].toString();
    mRoomName = roomObj[QLatin1StringView("name")].toString();
    mRoomFName = roomObj[QLatin1StringView("fname")].toString();
    // TODO
}

QString ModerationMessage::roomFName() const
{
    return mRoomFName;
}

void ModerationMessage::setRoomFName(const QString &newRoomFName)
{
    mRoomFName = newRoomFName;
}

bool ModerationMessage::isEmpty() const
{
    return mModerationId.isEmpty();
}

QString ModerationMessage::roomId() const
{
    return mRoomId;
}

void ModerationMessage::setRoomId(const QString &newRoomId)
{
    mRoomId = newRoomId;
}

QString ModerationMessage::roomName() const
{
    return mRoomName;
}

void ModerationMessage::setRoomName(const QString &newRoomName)
{
    mRoomName = newRoomName;
}

QDebug operator<<(QDebug d, const ModerationMessage &t)
{
    d.space() << "mRoomName" << t.roomName();
    d.space() << "mRoomId" << t.roomId();
    d.space() << "mRoomFName" << t.roomFName();
    return d;
}
