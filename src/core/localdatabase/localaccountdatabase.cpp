/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountdatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "ruqola_debug.h"

static const char s_schemaAccountDataBase[] = "CREATE TABLE ACCOUNT (roomId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class Fields {
    RoomId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalAccountDatabase::LocalAccountDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localAccountDatabasePath())
{
}

LocalAccountDatabase::~LocalAccountDatabase() = default;

QString LocalAccountDatabase::databaseName(const QString &name)
{
    return QStringLiteral("accounts-") + name;
}
