/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabaseutils.h"
using namespace Qt::Literals::StringLiterals;

#include <QDateTime>
#include <QStandardPaths>

QString LocalDatabaseUtils::fixRoomName(QString roomName)
{
    roomName.remove(u'/');
    roomName.remove(u':');
    return roomName;
}

QString LocalDatabaseUtils::localMessageLoggerPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/logs/"_s;
}

QString LocalDatabaseUtils::localDatabasePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + u"/database/"_s;
}

QString LocalDatabaseUtils::localMessagesDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Messages);
}

QString LocalDatabaseUtils::localRoomsDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Rooms);
}

QString LocalDatabaseUtils::localAccountsDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Accounts);
}

QString LocalDatabaseUtils::localGlobalDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::Global);
}

QString LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath pathType)
{
    switch (pathType) {
    case LocalDatabaseUtils::DatabasePath::Messages:
        return u"messages/"_s;
    case LocalDatabaseUtils::DatabasePath::Rooms:
        return u"rooms/"_s;
    case LocalDatabaseUtils::DatabasePath::Accounts:
        return u"account/"_s;
    case LocalDatabaseUtils::DatabasePath::Global:
        return u"global/"_s;
    }
    Q_UNREACHABLE();
    return {};
}

QString LocalDatabaseUtils::deleteMessage()
{
    return u"DELETE FROM MESSAGES WHERE messageId = ?"_s;
}

QString LocalDatabaseUtils::insertReplaceMessage()
{
    return u"INSERT OR REPLACE INTO MESSAGES VALUES (?, ?, ?)"_s;
}

QString LocalDatabaseUtils::deleteRoom()
{
    return u"DELETE FROM ROOMS WHERE roomId = ?"_s;
}

QString LocalDatabaseUtils::jsonRoom()
{
    return u"SELECT json FROM ROOMS WHERE roomId = \"%1\""_s;
}

QString LocalDatabaseUtils::insertReplaceRoom()
{
    return u"INSERT OR REPLACE INTO ROOMS VALUES (?, ?, ?)"_s;
}

QString LocalDatabaseUtils::deleteAccount()
{
    return u"DELETE FROM ACCOUNT WHERE accountName = ?"_s;
}

QString LocalDatabaseUtils::updateAccount()
{
    return u"INSERT OR REPLACE INTO ACCOUNT VALUES (?, ?)"_s;
}

QString LocalDatabaseUtils::insertReplaceGlobal()
{
    return u"INSERT OR REPLACE INTO GLOBAL VALUES (?, ?)"_s;
}

QString LocalDatabaseUtils::removeGlobal()
{
    return u"DELETE FROM GLOBAL WHERE identifier = ?"_s;
}

QString LocalDatabaseUtils::timestampGlobal()
{
    return u"SELECT timestamp FROM GLOBAL WHERE identifier = \"%1\""_s;
}

QString LocalDatabaseUtils::deleteMessageFromLogs()
{
    return u"DELETE FROM LOGS WHERE messageId = ?"_s;
}

QString LocalDatabaseUtils::insertReplaceMessageFromLogs()
{
    return u"INSERT OR REPLACE INTO LOGS VALUES (?, ?, ?, ?)"_s;
}

qint64 LocalDatabaseUtils::currentTimeStamp()
{
    return QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
}

QString LocalDatabaseUtils::jsonAccount()
{
    return u"SELECT json FROM ACCOUNT WHERE accountName = \"%1\""_s;
}
