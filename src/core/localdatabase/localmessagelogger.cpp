/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagelogger.h"

#include "localdatabaseutils.h"
#include "messages/message.h"
#include "ruqola_debug.h"
#include "ruqolaglobalconfig.h"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTextStream>

LocalMessageLogger::LocalMessageLogger()
    : mBasePath(LocalDatabaseUtils::localMessageLoggerPath())
{
}

static const char s_schema[] = "CREATE TABLE LOGS (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, userName TEXT, text TEXT)";
enum class Fields {
    MessageId,
    TimeStamp,
    UserName,
    Text,
}; // in the same order as the table

QString LocalMessageLogger::dbFileName(const QString &accountName, const QString &roomName) const
{
    const QString dirPath = mBasePath + accountName;
    return dirPath + QLatin1Char('/') + roomName + QStringLiteral(".sqlite");
}

void LocalMessageLogger::addMessage(const QString &accountName, const QString &_roomName, const Message &m)
{
    if (!RuqolaGlobalConfig::self()->enableLogging()) {
        return;
    }
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = accountName + QLatin1Char('-') + roomName;
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
            db.exec(QString::fromLatin1(s_schema));
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
    QSqlQuery query(QStringLiteral("INSERT OR REPLACE INTO LOGS VALUES (?, ?, ?, ?)"), db);
    query.addBindValue(m.messageId());
    query.addBindValue(m.timeStamp());
    query.addBindValue(m.username());
    query.addBindValue(m.text());
    if (!query.exec()) {
        qCWarning(RUQOLA_LOG) << "Couldn't insert-or-replace in LOGS table" << db.databaseName() << query.lastError();
    }
}

void LocalMessageLogger::deleteMessage(const QString &accountName, const QString &_roomName, const QString &messageId)
{
    if (!RuqolaGlobalConfig::self()->enableLogging()) {
        return;
    }
    // addMessage is always called before deleteMessage, if only for the history replay on connect
    // So the db must exist
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = accountName + QLatin1Char('-') + roomName;
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        qCWarning(RUQOLA_LOG) << "The assumption was wrong, deleteMessage was called before addMessage, in account" << accountName << "room" << roomName;
        return;
    }
    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    QSqlQuery query(QStringLiteral("DELETE FROM LOGS WHERE messageId = ?"), db);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qCWarning(RUQOLA_LOG) << "Couldn't insert-or-replace in LOGS table" << db.databaseName() << query.lastError();
    }
}

std::unique_ptr<QSqlTableModel> LocalMessageLogger::createMessageModel(const QString &accountName, const QString &_roomName) const
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = accountName + QLatin1Char('-') + roomName;
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomName);
        if (!QFileInfo::exists(fileName)) {
            return {};
        }
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), dbName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            qCWarning(RUQOLA_LOG) << "Couldn't open" << fileName;
            return {};
        }
    }

    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    auto model = std::make_unique<QSqlTableModel>(nullptr, db);
    model->setTable(QStringLiteral("LOGS"));
    model->setSort(int(Fields::TimeStamp), Qt::AscendingOrder);
    model->select();
    return model;
}

bool LocalMessageLogger::saveToFile(QFile &file, const QString &accountName, const QString &_roomName) const
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    auto model = createMessageModel(accountName, roomName);
    if (!model) {
        return false;
    }
    Q_ASSERT(file.isOpen());
    QTextStream stream(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    stream.setCodec("utf-8");
#endif

    int rows = model->rowCount();
    for (int row = 0; row < rows; ++row) {
        const QSqlRecord record = model->record(row);
        const QDateTime timeStamp = QDateTime::fromMSecsSinceEpoch(record.value(int(Fields::TimeStamp)).toULongLong());
        const QString userName = record.value(int(Fields::UserName)).toString();
        const QString text = record.value(int(Fields::Text)).toString();
        stream << "[" << timeStamp.toString(Qt::ISODate) << "] <" << userName << "> " << text << '\n';
        if (row == rows - 1 && model->canFetchMore()) {
            model->fetchMore();
            rows = model->rowCount();
        }
    }
    return true;
}
