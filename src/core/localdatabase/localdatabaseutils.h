/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QString>

namespace LocalDatabaseUtils
{
enum class DatabasePath : uint8_t {
    Messages,
    Rooms,
    Accounts,
    Global,
    E2E,
    RoomPendingTypedInfo,
    RoomSubscriptions,
};

[[nodiscard]] LIBRUQOLACORE_EXPORT QString fixRoomName(QString roomName);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localMessageLoggerPath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localMessagesDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localRoomsDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localAccountsDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localGlobalDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localE2EDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localRoomPendingTypedInfoDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localRoomSubscriptionsDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString databasePath(LocalDatabaseUtils::DatabasePath pathType);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteMessage();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceMessage();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteRoom();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString jsonRoom();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceRoom();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteAccount();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString updateAccount();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceGlobal();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString removeGlobal();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString timestampGlobal();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteMessageFromLogs();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceMessageFromLogs();
[[nodiscard]] LIBRUQOLACORE_EXPORT qint64 currentTimeStamp();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString jsonAccount();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceRoomPendingTypedInfo();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteRoomPendingTypedInfo();
};
