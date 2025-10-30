/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "texttospeechenqueueinfo.h"

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
