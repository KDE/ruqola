/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabaseutils.h"

#include <QDateTime>
#include <QStandardPaths>

using namespace Qt::Literals::StringLiterals;
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

QString LocalDatabaseUtils::localE2EDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::E2E);
}

QString LocalDatabaseUtils::localRoomPendingTypedInfoDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::RoomPendingTypedInfo);
}

QString LocalDatabaseUtils::localRoomSubscriptionsDatabasePath()
{
    return LocalDatabaseUtils::localDatabasePath() + LocalDatabaseUtils::databasePath(LocalDatabaseUtils::DatabasePath::RoomSubscriptions);
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
    case LocalDatabaseUtils::DatabasePath::E2E:
        return u"e2e/"_s;
    case LocalDatabaseUtils::DatabasePath::RoomPendingTypedInfo:
        return u"roompendingtypedinfo/"_s;
    case LocalDatabaseUtils::DatabasePath::RoomSubscriptions:
        return u"roomsubscriptions/"_s;
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
    return QDateTime::currentMSecsSinceEpoch();
}

QString LocalDatabaseUtils::jsonAccount()
{
    return u"SELECT json FROM ACCOUNT WHERE accountName = \"%1\""_s;
}

QString LocalDatabaseUtils::insertReplaceRoomPendingTypedInfo()
{
    return u"INSERT OR REPLACE INTO ROOMPENDINGTYPED VALUES (?, ?)"_s;
}

QString LocalDatabaseUtils::deleteRoomPendingTypedInfo()
{
    return u"DELETE FROM ROOMPENDINGTYPED WHERE roomId = ?"_s;
}

QString LocalDatabaseUtils::deleteRoomSubscription()
{
    return u"DELETE FROM ROOMSUBSCRIPTIONS WHERE subscriptionId = ?"_s;
}

QString LocalDatabaseUtils::insertRoomSubscription()
{
    return u"INSERT OR REPLACE INTO ROOMSUBSCRIPTIONS VALUES (?, ?)"_s;
}

QString LocalDatabaseUtils::roomIdFromSubscription()
{
    return u"SELECT roomId FROM ROOMSUBSCRIPTIONS WHERE subscriptionId = \"%1\""_s;
}
