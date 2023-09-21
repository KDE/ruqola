/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_EXPORT ParseMessageUrlUtils
{
    Q_GADGET
public:
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

    ParseMessageUrlUtils();
    ~ParseMessageUrlUtils();

    [[nodiscard]] bool parseUrl(const QString &messageUrl);

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

private:
    QString mMessageId;
    QString mRoomId;
    QString mServerHost;
    QString mPath;
    RoomIdType mRoomIdType = RoomIdType::Unknown;
    ChannelType mChannelType = ChannelType::Unknown;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ParseMessageUrlUtils &t);
