/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QString>

namespace LocalDatabaseUtils
{
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString fixRoomName(QString roomName);
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString localMessageLoggerPath();
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString localDatabasePath();
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString localMessagesDatabasePath();
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString localRoomsDatabasePath();
Q_REQUIRED_RESULT LIBRUQOLACORE_EXPORT QString localAccountDatabasePath();
};
