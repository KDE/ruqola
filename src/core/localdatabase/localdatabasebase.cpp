/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebase.h"
using namespace Qt::Literals::StringLiterals;

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
    return dirPath + u'/' + roomName + u".sqlite"_s;
}

QString LocalDatabaseBase::dbFileName(const QString &accountName, const QByteArray &roomId) const
{
    return dbFileName(accountName, QString::fromLatin1(roomId));
}

QString LocalDatabaseBase::dbFileName(const QString &accountName) const
{
    const QString dirPath = mBasePath + accountName;
    return dirPath + u'/' + accountName + u".sqlite"_s;
}

QString LocalDatabaseBase::schemaDatabaseStr() const
{
    return schemaDataBase();
}

QString LocalDatabaseBase::schemaDataBase() const
{
    Q_ASSERT(false);
    return {};
}

QString LocalDatabaseBase::databaseName(const QString &name) const
{
    QString prefix;
    switch (mDatabaseType) {
    case DatabaseType::Unknown:
        qCWarning(RUQOLA_DATABASE_LOG) << "Unknown data base it's a bug" << name;
        break;
    case DatabaseType::Accounts:
        prefix = u"accounts-"_s;
        break;
    case DatabaseType::Rooms:
        prefix = u"rooms-"_s;
        break;
    case DatabaseType::Messages:
        prefix = u"messages-"_s;
        break;
    case DatabaseType::Global:
        prefix = u"global-"_s;
        break;
    case DatabaseType::Logger:
        break;
    }
    return prefix + name;
}

void LocalDatabaseBase::setDatabaseLogger(RocketChatRestApi::AbstractLogger *logger)
{
    mRuqolaLogger = logger;
}

bool LocalDatabaseBase::checkDataBase(const QString &accountName, const QByteArray &roomId, QSqlDatabase &db)
{
    const QString dbName = generateDatabaseName(accountName, roomId);
    db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "The assumption was wrong, deleteMessage was called before addMessage, in account" << accountName << "room" << roomId
                                       << "database file " << dbName;
        return false;
    }
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
    Q_ASSERT(db.isOpen());
    return true;
}

QString LocalDatabaseBase::generateDatabaseName(const QString &accountName, const QByteArray &roomId) const
{
    const QString dbName = databaseName(accountName + u'-' + QString::fromLatin1(roomId));
    return dbName;
}

bool LocalDatabaseBase::initializeDataBase(const QString &accountName, const QByteArray &roomId, QSqlDatabase &db)
{
    const QString dbName = generateDatabaseName(accountName, roomId);
    db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase(u"QSQLITE"_s, dbName);
        const QString dirPath = mBasePath + accountName;
        if (!QDir().mkpath(dirPath)) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << dirPath;
            return false;
        }
        const QString fileName = dbFileName(accountName, roomId);
        const bool newDb = QFileInfo::exists(fileName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << db.databaseName();
            return false;
        }
        QSqlQuery query(db);
        if (!newDb) {
            query.exec(schemaDataBase());
            if (query.lastError().isValid()) {
                qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create table LOGS in" << db.databaseName() << ":" << db.lastError();
                return false;
            }
        }
        // Using the write-ahead log and sync = NORMAL for faster writes
        // (idea taken from kactivities-stat)
        query.exec(u"PRAGMA synchronous = 1"_s);
        // use the write-ahead log (requires sqlite > 3.7.0)
        query.exec(u"PRAGMA journal_mode = WAL"_s);
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
        db = QSqlDatabase::addDatabase(u"QSQLITE"_s, dbName);
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
        QSqlQuery query(db);
        if (!newDb) {
            query.exec(schemaDataBase());
            if (query.lastError().isValid()) {
                qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create table LOGS in" << db.databaseName() << ":" << db.lastError();
                return false;
            }
        }
        // Using the write-ahead log and sync = NORMAL for faster writes
        // (idea taken from kactivities-stat)
        query.exec(u"PRAGMA synchronous = 1"_s);
        // use the write-ahead log (requires sqlite > 3.7.0)
        query.exec(u"PRAGMA journal_mode = WAL"_s);
    }

    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    return true;
}
