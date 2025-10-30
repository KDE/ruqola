/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QString>
class TextToSpeechEnqueueInfo
{
public:
    TextToSpeechEnqueueInfo();
    ~TextToSpeechEnqueueInfo();

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    [[nodiscard]] QByteArray messageId() const;
    void setMessageId(const QByteArray &newMessageId);

    [[nodiscard]] QString accountName() const;
    void setAccountName(const QString &newAccountName);

private:
    QByteArray mMessageId;
    QByteArray mRoomId;
    QString mAccountName;
};
