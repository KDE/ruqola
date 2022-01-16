/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT ParseMessageUrlUtils
{
public:
    enum class ChannelType {
        Unknown = 0,
        Direct = 1,
        Channel = 2,
    };
    enum class RoomIdType {
        Unknown = 0,
        RoomId = 1,
        RoomName = 2,
    };

    ParseMessageUrlUtils();
    ~ParseMessageUrlUtils();

    Q_REQUIRED_RESULT bool parseUrl(const QString &messageUrl);

    const QString &messageId() const;
    void setMessageId(const QString &newMessageId);

    const QString &roomId() const;
    void setRoomId(const QString &newRoomId);

    const QString &serverHost() const;
    void setServerHost(const QString &newServerPath);

    const QString &path() const;
    void setPath(const QString &newPath);

private:
    QString mMessageId;
    QString mRoomId;
    QString mServerHost;
    QString mPath;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ParseMessageUrlUtils &t);
