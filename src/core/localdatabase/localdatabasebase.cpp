/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebase.h"

#include "ruqola_database_debug.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

using namespace Qt::Literals::StringLiterals;
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
    case DatabaseType::E2E:
        prefix = u"e2e-"_s;
        break;
    case DatabaseType::PendingTypedInfo:
        prefix = u"pendingtypedinfo-"_s;
        break;
    case DatabaseType::RoomSubscriptions:
        prefix = u"roomsubscriptions-"_s;
        break;
    case DatabaseType::E2ERooms:
        prefix = u"e2e-rooms-"_s;
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

void LocalDatabaseBase::forgetDataBase(const QString &dbName, QSqlDatabase &db)
{
    // removeDatabase() warns and keeps the connection alive as long as a QSqlDatabase copy exists,
    // so drop ours first.
    db = QSqlDatabase();
    if (QSqlDatabase::contains(dbName)) {
        QSqlDatabase::removeDatabase(dbName);
    }
}

bool LocalDatabaseBase::openOrCreateDataBase(const QString &dbName, const QString &dirPath, const QString &fileName, QSqlDatabase &db)
{
    db = QSqlDatabase::database(dbName);
    if (db.isValid() && db.isOpen()) {
        return true;
    }
    // Either there is no such connection yet, or a previous attempt failed halfway through (the
    // connection is registered but was never opened). Drop it: isValid() only tells us the driver
    // is there, so keeping it would make us skip the schema creation below forever.
    forgetDataBase(dbName, db);

    if (!QDir().mkpath(dirPath)) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << dirPath;
        return false;
    }
    const bool dbExists = QFileInfo::exists(fileName);
    db = QSqlDatabase::addDatabase(u"QSQLITE"_s, dbName);
    db.setDatabaseName(fileName);
    if (!db.open()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create" << db.databaseName();
        forgetDataBase(dbName, db);
        return false;
    }
    bool schemaFailed = false;
    {
        QSqlQuery query(db);
        if (!dbExists) {
            query.exec(schemaDataBase());
            if (query.lastError().isValid()) {
                qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't create table in" << db.databaseName() << ":" << db.lastError();
                schemaFailed = true;
            }
        }
        if (!schemaFailed) {
            // Using the write-ahead log and sync = NORMAL for faster writes
            // (idea taken from kactivities-stat)
            query.exec(u"PRAGMA synchronous = 1"_s);
            // use the write-ahead log (requires sqlite > 3.7.0)
            query.exec(u"PRAGMA journal_mode = WAL"_s);
        }
    }
    if (schemaFailed) {
        // open() has just created an empty file. Remove it along with the connection, otherwise the
        // next run would see an existing file, skip the schema creation and query missing tables.
        forgetDataBase(dbName, db);
        if (!QFile::remove(fileName)) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't remove incomplete database" << fileName;
        }
        return false;
    }

    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    return true;
}

bool LocalDatabaseBase::initializeDataBase(const QString &accountName, const QByteArray &roomId, QSqlDatabase &db)
{
    return openOrCreateDataBase(generateDatabaseName(accountName, roomId), mBasePath + accountName, dbFileName(accountName, roomId), db);
}

bool LocalDatabaseBase::initializeDataBase(const QString &accountName, QSqlDatabase &db)
{
    return openOrCreateDataBase(databaseName(accountName), mBasePath + accountName, dbFileName(accountName), db);
}
