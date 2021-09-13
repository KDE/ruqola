/*
   Copyright (c) 2021 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "localmessagelogger.h"

#include "ruqola_debug.h"
#include <messages/message.h>

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTextStream>

LocalMessageLogger::LocalMessageLogger()
    : m_basePath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + QStringLiteral("/logs/"))
{
}

static const char s_schema[] = "CREATE TABLE LOGS (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, userName TEXT, text TEXT)";
enum class Fields { MessageId, TimeStamp, UserName, Text }; // in the same order as the table

QString LocalMessageLogger::dbFileName(const QString &accountName, const QString &roomName) const
{
    const QString dirPath = m_basePath + accountName;
    return dirPath + QLatin1Char('/') + roomName + QStringLiteral(".sqlite");
}

void LocalMessageLogger::addMessage(const QString &accountName, const QString &roomName, const Message &m)
{
    const QString dbName = accountName + QLatin1Char('-') + roomName;
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), dbName);
        const QString dirPath = m_basePath + accountName;
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

void LocalMessageLogger::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
    // addMessage is always called before deleteMessage, if only for the history replay on connect
    // So the db must exist
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

std::unique_ptr<QSqlTableModel> LocalMessageLogger::createMessageModel(const QString &accountName, const QString &roomName) const
{
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

bool LocalMessageLogger::saveToFile(QFile &file, const QString &accountName, const QString &roomName) const
{
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
