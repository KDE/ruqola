/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "globaldatabase.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

static const char s_schemaGlobalDataBase[] = "CREATE TABLE GLOBAL (identifier TEXT PRIMARY KEY NOT NULL, timestamp INTEGER)";
enum class GlobalFields {
    Identifier,
    TimeStamp,
}; // in the same order as the table

GlobalDatabase::GlobalDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localGlobalDatabasePath(), LocalDatabaseBase::DatabaseType::Global)
{
}

GlobalDatabase::~GlobalDatabase() = default;

QString GlobalDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaGlobalDataBase);
}

QString GlobalDatabase::generateIdentifier(const QString &accountName, const QString &roomName, TimeStampType type)
{
    QString identifier;
    if (accountName.isEmpty()) {
        return identifier;
    }
    switch (type) {
    case TimeStampType::MessageTimeStamp:
        identifier = QStringLiteral("messages-");
        break;
    case TimeStampType::RoomTimeStamp:
        identifier = QStringLiteral("rooms-");
        break;
    case TimeStampType::AccountTimeStamp:
        identifier = QStringLiteral("account-");
        break;
    }
    identifier += accountName;
    if (roomName.isEmpty() && type != TimeStampType::AccountTimeStamp) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Missing roomName! It's a bug!!!";
    }
    if (!roomName.isEmpty()) {
        identifier += QLatin1Char('-') + LocalDatabaseUtils::fixRoomName(roomName);
    }
    return identifier;
}

void GlobalDatabase::insertOrReplaceTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, TimeStampType type)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        const QString identifier = generateIdentifier(accountName, roomName, type);
        QSqlQuery query(LocalDatabaseUtils::insertReplaceGlobal(), db);
        query.addBindValue(identifier);
        query.addBindValue(timestamp);
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
        }
    }
}

void GlobalDatabase::removeTimeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    const QString identifier = generateIdentifier(accountName, roomName, type);
    QSqlQuery query(QStringLiteral("DELETE FROM GLOBAL WHERE identifier = ?"), db);
    query.addBindValue(identifier);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
    }
}

qint64 GlobalDatabase::timeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return -1;
    }
    const QString identifier = generateIdentifier(accountName, roomName, type);
    QSqlQuery query(QStringLiteral("SELECT timestamp FROM GLOBAL WHERE identifier = \"%1\"").arg(identifier), db);
    qint64 value = -1;
    // We have one element
    if (query.first()) {
        value = query.value(0).toLongLong();
    }
    return value;
}
