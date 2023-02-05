/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountdatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

static const char s_schemaAccountDataBase[] = "CREATE TABLE ACCOUNT (roomId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class Fields {
    RoomId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalAccountDatabase::LocalAccountDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localAccountDatabasePath(), LocalDatabaseBase::DatabaseType::Account)
{
}

LocalAccountDatabase::~LocalAccountDatabase() = default;

QString LocalAccountDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaAccountDataBase);
}
