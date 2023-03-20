/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagelogger.h"

#include "localdatabaseutils.h"
#include "messages/message.h"
#include "ruqola_database_debug.h"
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
    : LocalDatabaseBase(LocalDatabaseUtils::localMessageLoggerPath(), LocalDatabaseBase::DatabaseType::Logger)
{
}

static const char s_schema[] = "CREATE TABLE LOGS (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, userName TEXT, text TEXT)";
enum class Fields {
    MessageId,
    TimeStamp,
    UserName,
    Text,
}; // in the same order as the table

QString LocalMessageLogger::schemaDataBase() const
{
    return QString::fromLatin1(s_schema);
}

void LocalMessageLogger::addMessage(const QString &accountName, const QString &_roomName, const Message &m)
{
    if (!RuqolaGlobalConfig::self()->enableLogging()) {
        return;
    }
    QSqlDatabase db;
    if (initializeDataBase(accountName, _roomName, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertReplaceMessageFromLogs(), db);
        query.addBindValue(m.messageId());
        query.addBindValue(m.timeStamp());
        query.addBindValue(m.username());
        query.addBindValue(m.text());
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in LOGS table" << db.databaseName() << query.lastError();
        }
    }
}

void LocalMessageLogger::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
    if (!RuqolaGlobalConfig::self()->enableLogging()) {
        return;
    }
    QSqlDatabase db;
    if (!checkDataBase(accountName, roomName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteMessageFromLogs(), db);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in LOGS table" << db.databaseName() << query.lastError();
    }
}

std::unique_ptr<QSqlTableModel> LocalMessageLogger::createMessageModel(const QString &accountName, const QString &_roomName) const
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
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
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't open" << fileName;
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
