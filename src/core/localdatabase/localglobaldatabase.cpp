/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localglobaldatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"

static const char s_schemaAccountDataBase[] = "CREATE TABLE GLOBAL (identifier TEXT PRIMARY KEY NOT NULL, timestamp INTEGER)";
enum class Fields {
    Identifier,
    TimeStamp,
}; // in the same order as the table

LocalGlobalDatabase::LocalGlobalDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localAccountDatabasePath(), LocalDatabaseBase::DatabaseType::Account)
{
}

LocalGlobalDatabase::~LocalGlobalDatabase() = default;

QString LocalGlobalDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaAccountDataBase);
}
