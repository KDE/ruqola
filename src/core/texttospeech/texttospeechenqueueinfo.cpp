/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "texttospeechenqueueinfo.h"
#include <QDebug>

TextToSpeechEnqueueInfo::TextToSpeechEnqueueInfo() = default;

TextToSpeechEnqueueInfo::~TextToSpeechEnqueueInfo() = default;

QByteArray TextToSpeechEnqueueInfo::roomId() const
{
    return mRoomId;
}

void TextToSpeechEnqueueInfo::setRoomId(const QByteArray &newRoomId)
{
    mRoomId = newRoomId;
}

QByteArray TextToSpeechEnqueueInfo::messageId() const
{
    return mMessageId;
}

void TextToSpeechEnqueueInfo::setMessageId(const QByteArray &newMessageId)
{
    mMessageId = newMessageId;
}

QString TextToSpeechEnqueueInfo::accountName() const
{
    return mAccountName;
}

void TextToSpeechEnqueueInfo::setAccountName(const QString &newAccountName)
{
    mAccountName = newAccountName;
}

bool TextToSpeechEnqueueInfo::operator==(const TextToSpeechEnqueueInfo &other) const
{
    return mRoomId == other.roomId() && mAccountName == other.accountName() && mMessageId == other.messageId();
}

bool TextToSpeechEnqueueInfo::isValid() const
{
    return !mRoomId.isEmpty() && !mAccountName.isEmpty() && !mMessageId.isEmpty();
}

QDebug operator<<(QDebug d, const TextToSpeechEnqueueInfo &t)
{
    d.space() << "roomId:" << t.roomId();
    d.space() << "messageId:" << t.messageId();
    d.space() << "accountName:" << t.accountName();
    return d;
}
