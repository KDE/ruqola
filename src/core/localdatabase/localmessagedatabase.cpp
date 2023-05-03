/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagedatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "messages/message.h"
#include "ruqola_database_debug.h"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

static const char s_schemaMessageDataBase[] = "CREATE TABLE MESSAGES (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class MessagesFields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalMessageDatabase::LocalMessageDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localMessagesDatabasePath(), LocalDatabaseBase::DatabaseType::Message)
{
}

LocalMessageDatabase::~LocalMessageDatabase() = default;

QString LocalMessageDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaMessageDataBase);
}

void LocalMessageDatabase::addMessage(const QString &accountName, const QString &roomName, const Message &m)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (initializeDataBase(accountName, roomName, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertReplaceMessages(), db);
        query.addBindValue(m.messageId());
        query.addBindValue(m.timeStamp());
        // TODO look at why we can't save a binary ?
        query.addBindValue(Message::serialize(m, false)); // TODO binary or not ?
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
        }
    }
#endif
}

void LocalMessageDatabase::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (!checkDataBase(accountName, roomName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteMessage(), db);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
    }
#endif
}

// TODO add autotests
QVector<QString> LocalMessageDatabase::loadMessages(const QString &accountName, const QString &_roomName, int startId, int endId, int numberElements) const
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomName);
        qDebug() << " fileName " << fileName;
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
    const QString query = QStringLiteral("SELECT * FROM MESSAGES WHERE timestamp >= :startId AND timestamp <= :endId LIMIT :limit");
    QSqlQuery resultQuery(db);
    resultQuery.prepare(query);
    resultQuery.bindValue(QStringLiteral(":startId"), startId);
    resultQuery.bindValue(QStringLiteral(":endId"), endId);
    resultQuery.bindValue(QStringLiteral(":limit"), numberElements);
    resultQuery.exec();
    QVector<QString> result;
    while (resultQuery.next()) {
        const QString json = resultQuery.value(QStringLiteral("json")).toString();
        result.append(json);
    }

    return result;
}

std::unique_ptr<QSqlTableModel> LocalMessageDatabase::createMessageModel(const QString &accountName, const QString &_roomName) const
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomName);
        qDebug() << " fileName " << fileName;
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
    model->setTable(QStringLiteral("MESSAGES"));
    model->setSort(int(MessagesFields::TimeStamp), Qt::AscendingOrder);
    model->select();
    return model;
}
