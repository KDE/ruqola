/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localroomsdatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "room.h"
#include "ruqola_database_debug.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

static const char s_schemaRoomDataBase[] = "CREATE TABLE ROOMS (roomId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class RoomFields {
    RoomId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalRoomsDatabase::LocalRoomsDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localRoomsDatabasePath(), LocalDatabaseBase::DatabaseType::Rooms)
{
}

LocalRoomsDatabase::~LocalRoomsDatabase() = default;

QString LocalRoomsDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaRoomDataBase);
}

void LocalRoomsDatabase::addRoom(const QString &accountName, Room *room)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertReplaceRoom(), db);
        query.addBindValue(room->roomId());
        query.addBindValue(room->updatedAt()); // TODO ?
        query.addBindValue(Room::serialize(room)); // TODO use binary ?
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMS table" << db.databaseName() << query.lastError();
        }
    }
#else
    Q_UNUSED(accountName)
    Q_UNUSED(room)
#endif
}

void LocalRoomsDatabase::deleteRoom(const QString &accountName, const QString &roomId)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteRooms(), db);
    query.addBindValue(roomId);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMS table" << db.databaseName() << query.lastError();
    }
#else
    Q_UNUSED(accountName)
    Q_UNUSED(roomId)
#endif
}
