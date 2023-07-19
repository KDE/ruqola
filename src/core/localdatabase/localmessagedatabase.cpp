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
#include <QJsonDocument>
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
        // qDebug() << " m.timeStamp() " << m.timeStamp();
        // TODO look at why we can't save a binary ?
        query.addBindValue(Message::serialize(m, false)); // TODO binary or not ?
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
        }
    }
#else
    Q_UNUSED(accountName)
    Q_UNUSED(roomName)
    Q_UNUSED(m)
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
#else
    Q_UNUSED(accountName)
    Q_UNUSED(roomName)
    Q_UNUSED(messageId)
#endif
}

QString LocalMessageDatabase::generateQueryStr(qint64 startId, qint64 endId, qint64 numberElements)
{
    QString query = QStringLiteral("SELECT * FROM MESSAGES");
    if (startId != -1) {
        query += QStringLiteral(" WHERE timestamp >= :startId");
        if (endId != -1) {
            query += QStringLiteral(" AND timestamp <= :endId");
        }
    } else {
        if (endId != -1) {
            query += QStringLiteral(" WHERE timestamp <= :endId");
        }
    }
    query += QStringLiteral(" ORDER BY timestamp DESC");

    if (numberElements != -1) {
        query += QStringLiteral(" LIMIT :limit");
    }
    return query;
}

QVector<Message>
LocalMessageDatabase::loadMessages(const QString &accountName, const QString &_roomName, qint64 startId, qint64 endId, qint64 numberElements) const
{
#if 0
    SELECT id, nom, email
    FROM utilisateurs
    LIMIT 5 OFFSET 5;

    // Use sorting ASC or DESC
#endif

    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomName);
        // qDebug() << " fileName " << fileName;
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
    const QString query = LocalMessageDatabase::generateQueryStr(startId, endId, numberElements);
    QSqlQuery resultQuery(db);
    resultQuery.prepare(query);
    if (startId != -1) {
        resultQuery.bindValue(QStringLiteral(":startId"), startId);
        if (endId != -1) {
            resultQuery.bindValue(QStringLiteral(":endId"), endId);
        }
    } else {
        if (endId != -1) {
            resultQuery.bindValue(QStringLiteral(":endId"), endId);
        }
    }
    if (numberElements != -1) {
        resultQuery.bindValue(QStringLiteral(":limit"), numberElements);
    }
    if (!resultQuery.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << " Impossible to execute query: " << resultQuery.lastError() << " query: " << query;
        return {};
    }

    QVector<Message> listMessages;
    while (resultQuery.next()) {
        const QString json = resultQuery.value(QStringLiteral("json")).toString();
        listMessages.append(convertJsonToMessage(json));
    }

    return listMessages;
}

Message LocalMessageDatabase::convertJsonToMessage(const QString &json)
{
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    const Message msg = Message::deserialize(doc.object());
    return msg;
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
