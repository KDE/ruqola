/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_EXPORT ParseRocketChatUrlUtils
{
    Q_GADGET
public:
    enum class UrlType {
        Unknown = 0,
        Message = 1,
        Server = 2,
        Invite = 3,
        ConferenceCall = 4,
        Room = 5,
    };
    Q_ENUM(UrlType)

    enum class ChannelType {
        Unknown = 0,
        Direct = 1,
        Channel = 2,
        Group = 3,
    };
    Q_ENUM(ChannelType)
    enum class RoomIdType {
        Unknown = 0,
        RoomId = 1,
        RoomName = 2,
    };
    Q_ENUM(RoomIdType)

    ParseRocketChatUrlUtils();
    ~ParseRocketChatUrlUtils();

    [[nodiscard]] UrlType parseUrl(const QString &messageUrl);

    [[nodiscard]] const QString &messageId() const;
    void setMessageId(const QString &newMessageId);

    [[nodiscard]] const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] const QString &serverHost() const;
    void setServerHost(const QString &newServerPath);

    [[nodiscard]] const QString &path() const;
    void setPath(const QString &newPath);

    [[nodiscard]] RoomIdType roomIdType() const;
    void setRoomIdType(RoomIdType newRoomIdType);

    [[nodiscard]] ChannelType channelType() const;
    void setChannelType(ChannelType newChannelType);

    [[nodiscard]] QString token() const;
    void setToken(const QString &newToken);

    [[nodiscard]] QString userId() const;
    void setUserId(const QString &newUserId);

private:
    QString mUserId;
    QString mToken;
    QString mMessageId;
    QString mRoomId;
    QString mServerHost;
    QString mPath;
    RoomIdType mRoomIdType = RoomIdType::Unknown;
    ChannelType mChannelType = ChannelType::Unknown;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ParseRocketChatUrlUtils &t);
