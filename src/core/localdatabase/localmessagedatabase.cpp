/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagedatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "messages/message.h"
#include "ruqola_debug.h"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

static const char s_schemaMessageDataBase[] = "CREATE TABLE MESSAGES (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class Fields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalMessageDatabase::LocalMessageDatabase()
    : mBasePath(LocalDatabaseUtils::localMessageDatabasePath())
{
}

LocalMessageDatabase::~LocalMessageDatabase() = default;

QString LocalMessageDatabase::dbFileName(const QString &accountName, const QString &roomName) const
{
    const QString dirPath = mBasePath + accountName;
    return dirPath + QLatin1Char('/') + roomName + QStringLiteral(".sqlite");
}

QString LocalMessageDatabase::databaseName(const QString &name)
{
    return QStringLiteral("messages-") + name;
}

void LocalMessageDatabase::addMessage(const QString &accountName, const QString &_roomName, const Message &m)
{
#if HAVE_DATABASE_SUPPORT
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), dbName);
        const QString dirPath = mBasePath + accountName;
        if (!QDir().mkpath(dirPath)) {
            qCWarning(RUQOLA_LOG) << "Couldn't create" << dirPath;
            return;
        }
        const QString fileName = dbFileName(accountName, roomName);
        const bool newDb = QFileInfo::exists(fileName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            qCWarning(RUQOLA_LOG) << "Couldn't create" << db.databaseName();
            return;
        }
        if (!newDb) {
            db.exec(QString::fromLatin1(s_schemaMessageDataBase));
            if (db.lastError().isValid()) {
                qCWarning(RUQOLA_LOG) << "Couldn't create table LOGS in" << db.databaseName() << ":" << db.lastError();
                return;
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
    QSqlQuery query(QStringLiteral("INSERT OR REPLACE INTO MESSAGES VALUES (?, ?, ?)"), db);
    query.addBindValue(m.messageId());
    query.addBindValue(m.timeStamp());
    query.addBindValue(m.text()); // TODO replace by json
    if (!query.exec()) {
        qCWarning(RUQOLA_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
    }

#endif
}

void LocalMessageDatabase::deleteMessage(const QString &accountName, const QString &_roomName, const QString &messageId)
{
#ifdef HAVE_DATABASE_SUPPORT
    // addMessage is always called before deleteMessage, if only for the history replay on connect
    // So the db must exist
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        qCWarning(RUQOLA_LOG) << "The assumption was wrong, deleteMessage was called before addMessage, in account" << accountName << "room" << roomName;
        return;
    }
    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    QSqlQuery query(QStringLiteral("DELETE FROM MESSAGES WHERE messageId = ?"), db);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qCWarning(RUQOLA_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
    }

#endif
}
