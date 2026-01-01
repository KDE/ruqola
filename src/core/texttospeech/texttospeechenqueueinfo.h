/*
  SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QString>
class LIBRUQOLACORE_EXPORT TextToSpeechEnqueueInfo
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

    [[nodiscard]] bool operator==(const TextToSpeechEnqueueInfo &other) const;

    [[nodiscard]] bool isValid() const;

private:
    QByteArray mMessageId;
    QByteArray mRoomId;
    QString mAccountName;
};
Q_DECLARE_TYPEINFO(TextToSpeechEnqueueInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const TextToSpeechEnqueueInfo &t);
