/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessage.h"

ModerationMessage::ModerationMessage(EmojiManager *emojiManager)
    : Message(emojiManager)
{
}

ModerationMessage::~ModerationMessage() = default;

QString ModerationMessage::moderationId() const
{
    return mModerationId;
}

void ModerationMessage::setModerationId(const QString &newModerationId)
{
    mModerationId = newModerationId;
}

void ModerationMessage::parse(const QJsonObject &message)
{
    mModerationId = message[QLatin1String("_id")].toString();
    // TODO add timestamp
    const QJsonObject messageObject = message[QLatin1String("message")].toObject();
    Message::parseMessage(messageObject, true);
    parseRoom(message[QLatin1String("room")].toObject());
}

void ModerationMessage::parseRoom(const QJsonObject &roomObj)
{
    // TODO
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
