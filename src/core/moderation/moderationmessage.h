/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "messages/message.h"
class EmojiManager;
class LIBRUQOLACORE_EXPORT ModerationMessage : public Message
{
public:
    explicit ModerationMessage(EmojiManager *emojiManager = nullptr);
    ~ModerationMessage();

    Q_REQUIRED_RESULT QString moderationId() const;
    void setModerationId(const QString &newModerationId);

    void parse(const QJsonObject &message);

    Q_REQUIRED_RESULT QString roomId() const;
    void setRoomId(const QString &newRoomId);

    Q_REQUIRED_RESULT QString roomName() const;
    void setRoomName(const QString &newRoomName);

private:
    LIBRUQOLACORE_NO_EXPORT void parseRoom(const QJsonObject &roomObj);
    QString mModerationId;
    QString mRoomId;
    QString mRoomName;
};
