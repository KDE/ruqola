/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabaseutils.h"

#include <QDateTime>
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
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Messages);
}

QString LocalDatabaseUtils::localRoomsDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Rooms);
}

QString LocalDatabaseUtils::localAccountDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Account);
}

QString LocalDatabaseUtils::localGlobalDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Global);
}

QString LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath pathType)
{
    switch (pathType) {
    case LocalDatabaseUtils::DatabasePath::Messages:
        return QStringLiteral("messages/");
    case LocalDatabaseUtils::DatabasePath::Rooms:
        return QStringLiteral("rooms/");
    case LocalDatabaseUtils::DatabasePath::Account:
        return QStringLiteral("account/");
    case LocalDatabaseUtils::DatabasePath::Global:
        return QStringLiteral("global/");
    }
    Q_UNREACHABLE();
    return {};
}

QString LocalDatabaseUtils::deleteMessage()
{
    return QStringLiteral("DELETE FROM MESSAGES WHERE messageId = ?");
}

QString LocalDatabaseUtils::insertReplaceMessages()
{
    return QStringLiteral("INSERT OR REPLACE INTO MESSAGES VALUES (?, ?, ?)");
}

QString LocalDatabaseUtils::deleteRooms()
{
    return QStringLiteral("DELETE FROM ROOMS WHERE roomId = ?");
}

QString LocalDatabaseUtils::insertReplaceRoom()
{
    return QStringLiteral("INSERT OR REPLACE INTO ROOMS VALUES (?, ?, ?)");
}

QString LocalDatabaseUtils::deleteAccount()
{
    return QStringLiteral("DELETE FROM ACCOUNT WHERE accountName = ?");
}

QString LocalDatabaseUtils::updateAccount()
{
    return QStringLiteral("INSERT OR REPLACE INTO ACCOUNT VALUES (?, ?)");
}

QString LocalDatabaseUtils::insertReplaceGlobal()
{
    return QStringLiteral("INSERT OR REPLACE INTO GLOBAL VALUES (?, ?)");
}

QString LocalDatabaseUtils::deleteMessageFromLogs()
{
    return QStringLiteral("DELETE FROM LOGS WHERE messageId = ?");
}

QString LocalDatabaseUtils::insertReplaceMessageFromLogs()
{
    return QStringLiteral("INSERT OR REPLACE INTO LOGS VALUES (?, ?, ?, ?)");
}

qint64 LocalDatabaseUtils::currentTimeStamp()
{
    return QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
}

QString LocalDatabaseUtils::jsonAccount()
{
    return QStringLiteral("SELECT json FROM ACCOUNT WHERE accountName = \"%1\"");
}
