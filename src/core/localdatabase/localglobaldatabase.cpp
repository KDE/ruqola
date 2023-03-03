/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localglobaldatabase.h"
#include "config-ruqola.h"
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
    case TimeStampType::AccountTimeStamp:
        identifier = QStringLiteral("account-");
        break;
    }
    return identifier + accountName + QLatin1Char('-') + LocalDatabaseUtils::fixRoomName(roomName);
}

void LocalGlobalDatabase::updateTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, TimeStampType type)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        const QString identifier = generateIdentifier(accountName, roomName, type);
        QSqlQuery query(QStringLiteral("INSERT OR REPLACE INTO GLOBAL VALUES (?, ?)"), db);
        query.addBindValue(identifier);
        query.addBindValue(timestamp);
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
        }
    }
#endif
}

void LocalGlobalDatabase::removeTimeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    const QString identifier = generateIdentifier(accountName, roomName, type);
    QSqlQuery query(QStringLiteral("DELETE FROM GLOBAL WHERE %1 = ?").arg(identifier), db);
    query.addBindValue(identifier);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
    }
#endif
}

qint64 LocalGlobalDatabase::timeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
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
#else
    Q_UNUSED(accountName)
    Q_UNUSED(roomName)
    Q_UNUSED(type)
    return -1;
#endif
}
