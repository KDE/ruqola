/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebase.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"

#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

LocalDatabaseBase::LocalDatabaseBase(const QString &basePath, LocalDatabaseBase::DatabaseType type)
    : mBasePath(basePath)
    , mDatabaseType(type)
{
}

LocalDatabaseBase::~LocalDatabaseBase() = default;

QString LocalDatabaseBase::dbFileName(const QString &accountName, const QString &roomName) const
{
    const QString dirPath = mBasePath + accountName;
    return dirPath + QLatin1Char('/') + roomName + QStringLiteral(".sqlite");
}

QString LocalDatabaseBase::dbFileName(const QString &accountName) const
{
    const QString dirPath = mBasePath;
    return dirPath + QLatin1Char('/') + accountName + QStringLiteral(".sqlite");
}

QString LocalDatabaseBase::schemaDataBase() const
{
    Q_ASSERT(true);
    return {};
}

QString LocalDatabaseBase::databaseName(const QString &name) const
{
    QString prefix;
    switch (mDatabaseType) {
    case DatabaseType::Unknown:
        qCWarning(RUQOLA_DATABASE_LOG) << "Unknown data base it's a bug";
        break;
    case DatabaseType::Account:
        prefix = QStringLiteral("accounts-");
    case DatabaseType::Rooms:
        prefix = QStringLiteral("rooms-");
        break;
    case DatabaseType::Message:
        prefix = QStringLiteral("messages-");
        break;
    case DatabaseType::Global:
        prefix = QStringLiteral("global-");
        break;
    case DatabaseType::Logger:
        break;
    }

    return prefix + name;
}

bool LocalDatabaseBase::checkDataBase(const QString &accountName, const QString &_roomName, QSqlDatabase &db)
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "The assumption was wrong, deleteMessage was called before addMessage, in account" << accountName << "room"
                                       << roomName << "database file " << dbName;
        return false;
    }
    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    return true;
}

bool LocalDatabaseBase::checkDataBase(const QString &accountName, QSqlDatabase &db)
{
    const QString dbName = databaseName(accountName);
    db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "The assumption was wrong, deleteMessage was called before addMessage, in account" << accountName << "database file "
                                       << dbName;
        return false;
    }
    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    return true;
}

bool LocalDatabaseBase::initializeDataBase(const QString &accountName, const QString &_roomName, QSqlDatabase &db)
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), dbName);
        const QString dirPath = mBasePath + accountName;
        if (!QDir().mkpath(dirPath)) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << dirPath;
            return false;
        }
        const QString fileName = dbFileName(accountName, roomName);
        const bool newDb = QFileInfo::exists(fileName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << db.databaseName();
            return false;
        }
        if (!newDb) {
            db.exec(schemaDataBase());
            if (db.lastError().isValid()) {
                qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create table LOGS in" << db.databaseName() << ":" << db.lastError();
                return false;
            }
        }
        // Using the write-ahead log and sync = NORMAL for faster writes
        // (idea taken from kactivities-stat)
        db.exec(QStringLiteral("PRAGMA synchronous = 1"));
        // use the write-ahead log (requires sqlite > 3.7.0)
        db.exec(QStringLiteral("PRAGMA journal_mode = WAL"));
    }

    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    return true;
}

bool LocalDatabaseBase::initializeDataBase(const QString &accountName, QSqlDatabase &db)
{
    const QString dbName = databaseName(accountName);
    db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), dbName);
        const QString dirPath = mBasePath + accountName;
        if (!QDir().mkpath(dirPath)) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << dirPath;
            return false;
        }
        const QString fileName = dbFileName(accountName);
        const bool newDb = QFileInfo::exists(fileName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << db.databaseName();
            return false;
        }
        if (!newDb) {
            db.exec(schemaDataBase());
            if (db.lastError().isValid()) {
                qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create table LOGS in" << db.databaseName() << ":" << db.lastError();
                return false;
            }
        }
        // Using the write-ahead log and sync = NORMAL for faster writes
        // (idea taken from kactivities-stat)
        db.exec(QStringLiteral("PRAGMA synchronous = 1"));
        // use the write-ahead log (requires sqlite > 3.7.0)
        db.exec(QStringLiteral("PRAGMA journal_mode = WAL"));
    }

    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    return true;
}
