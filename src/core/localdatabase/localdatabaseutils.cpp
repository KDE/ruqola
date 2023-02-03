/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabaseutils.h"

#include <QStandardPaths>

QString LocalDatabaseUtils::fixRoomName(QString roomName)
{
    roomName.remove(QLatin1Char('/'));
    roomName.remove(QLatin1Char(':'));
    return roomName;
}

QString LocalDatabaseUtils::localMessageLoggerPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/logs/");
}

QString LocalDatabaseUtils::localDatabasePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/database/");
}

QString LocalDatabaseUtils::localMessagesDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + QStringLiteral("messages/");
}

QString LocalDatabaseUtils::localRoomsDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + QStringLiteral("rooms/");
}

QString LocalDatabaseUtils::localAccountDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + QStringLiteral("account/");
}
