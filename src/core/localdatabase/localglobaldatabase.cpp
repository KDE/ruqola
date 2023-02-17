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
    : LocalDatabaseBase(LocalDatabaseUtils::localGlobalDatabasePath(), LocalDatabaseBase::DatabaseType::Global)
{
}

LocalGlobalDatabase::~LocalGlobalDatabase() = default;

QString LocalGlobalDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaAccountDataBase);
}

QString LocalGlobalDatabase::generateIdentifier(const QString &accountName, const QString &roomName, TimeStampType type)
{
    QString identifier;
    switch (type) {
    case TimeStampType::MessageTimeStamp:
        identifier = QStringLiteral("messages-");
        break;
    case TimeStampType::RoomTimeStamp:
        identifier = QStringLiteral("rooms-");
        break;
    }
    return identifier + accountName + QLatin1Char('-') + LocalDatabaseUtils::fixRoomName(roomName);
}

// TODO improve for identifier => account name/room name.
void LocalGlobalDatabase::updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, TimeStampType type)
{
    const QString identifier = generateIdentifier(accountName, roomName, type);

    // TODO
}

void LocalGlobalDatabase::removeTimeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
    const QString identifier = generateIdentifier(accountName, roomName, type);

    // TODO
}

qint64 LocalGlobalDatabase::timeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
    // TODO
    const QString identifier = generateIdentifier(accountName, roomName, type);
    return {};
}
