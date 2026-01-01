/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessage.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
ModerationMessage::ModerationMessage()

{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " ModerationMessage created " << this;
}

ModerationMessage::~ModerationMessage()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " ModerationMessage deleted " << this;
}

ModerationMessage::ModerationMessage(const ModerationMessage &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " ModerationMessage(const ModerationMessage &other) created " << this;
    mModerationId = other.moderationId();
    mRoomId = other.roomId();
    mRoomName = other.roomName();
    mRoomFName = other.roomFName();
}

bool ModerationMessage::operator==(const ModerationMessage &other) const
{
    return mModerationId == other.moderationId() && mRoomId == other.roomId() && mRoomName == other.roomName() && mRoomFName == other.roomFName();
}

QByteArray ModerationMessage::moderationId() const
{
    return mModerationId;
}

void ModerationMessage::setModerationId(const QByteArray &newModerationId)
{
    mModerationId = newModerationId;
}

void ModerationMessage::parse(const QJsonObject &obj)
{
    mModerationId = obj["_id"_L1].toString().toLatin1();
    // TODO add timestamp
    // const QJsonObject messageObject = obj["message"_L1].toObject();
    parseRoom(obj["room"_L1].toObject());
}

void ModerationMessage::parseRoom(const QJsonObject &roomObj)
{
    mRoomId = roomObj["_id"_L1].toString().toLatin1();
    mRoomName = roomObj["name"_L1].toString();
    mRoomFName = roomObj["fname"_L1].toString();
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

QByteArray ModerationMessage::roomId() const
{
    return mRoomId;
}

void ModerationMessage::setRoomId(const QByteArray &newRoomId)
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
