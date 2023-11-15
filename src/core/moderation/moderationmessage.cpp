/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
    mModerationId = obj[QLatin1String("_id")].toString();
    // TODO add timestamp
    const QJsonObject messageObject = obj[QLatin1String("message")].toObject();
    parseRoom(obj[QLatin1String("room")].toObject());
}

void ModerationMessage::parseRoom(const QJsonObject &roomObj)
{
    mRoomId = roomObj[QLatin1String("_id")].toString();
    mRoomName = roomObj[QLatin1String("name")].toString();
    mRoomFName = roomObj[QLatin1String("fname")].toString();
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
    d << " mRoomName " << t.roomName();
    d << " mRoomId " << t.roomId();
    d << " mRoomFName " << t.roomFName();
    return d;
}
