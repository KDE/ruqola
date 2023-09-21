/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QString>

namespace LocalDatabaseUtils
{
enum class DatabasePath {
    Messages,
    Rooms,
    Account,
    Global,
};

[[nodiscard]] LIBRUQOLACORE_EXPORT QString fixRoomName(QString roomName);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localMessageLoggerPath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localMessagesDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localRoomsDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localAccountDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString localGlobalDatabasePath();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString databasePath(LocalDatabaseUtils::DatabasePath pathType);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteMessage();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceMessages();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteRooms();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceRoom();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteAccount();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString updateAccount();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceGlobal();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString deleteMessageFromLogs();
[[nodiscard]] LIBRUQOLACORE_EXPORT QString insertReplaceMessageFromLogs();
[[nodiscard]] LIBRUQOLACORE_EXPORT qint64 currentTimeStamp();
};
