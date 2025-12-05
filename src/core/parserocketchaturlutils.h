/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QMetaType>
#include <QString>
class LIBRUQOLACORE_EXPORT ParseRocketChatUrlUtils
{
    Q_GADGET
public:
    enum class UrlType : uint8_t {
        Unknown = 0,
        Message = 1,
        Server = 2,
        Invite = 3,
        ConferenceCall = 4,
        Room = 5,
    };
    Q_ENUM(UrlType)

    enum class ChannelType : uint8_t {
        Unknown = 0,
        Direct = 1,
        Channel = 2,
        Group = 3,
    };
    Q_ENUM(ChannelType)
    enum class RoomIdType : uint8_t {
        Unknown = 0,
        RoomId = 1,
        RoomName = 2,
    };
    Q_ENUM(RoomIdType)

    struct ParsingInfo {
        QByteArray userId;
        QString token;
        QString messageId;
        QString roomId;
        QString serverHost;
        QString path;
        RoomIdType roomIdType = RoomIdType::Unknown;
        ChannelType channelType = ChannelType::Unknown;
    };

    ParseRocketChatUrlUtils();
    ~ParseRocketChatUrlUtils();

    [[nodiscard]] UrlType parseUrl(const QString &messageUrl);

    [[nodiscard]] ParsingInfo parsingInfo() const;
    void setParsingInfo(const ParsingInfo &newParsingInfo);

private:
    ParsingInfo mParsingInfo;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ParseRocketChatUrlUtils::ParsingInfo &t);
class QDebug;
Q_DECLARE_TYPEINFO(ParseRocketChatUrlUtils::ParsingInfo, Q_RELOCATABLE_TYPE);
