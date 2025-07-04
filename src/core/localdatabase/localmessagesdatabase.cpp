/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagesdatabase.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabaseutils.h"
#include "messages/message.h"
#include "rocketchataccount.h"
#include "ruqola_database_debug.h"

#include <QDir>
#include <QJsonDocument>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

static const char s_schemaMessagesDataBase[] = "CREATE TABLE MESSAGES (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class MessagesFields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalMessagesDatabase::LocalMessagesDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localMessagesDatabasePath(), LocalDatabaseBase::DatabaseType::Messages)
{
}

LocalMessagesDatabase::~LocalMessagesDatabase() = default;

QString LocalMessagesDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaMessagesDataBase);
}

void LocalMessagesDatabase::addMessage(const QString &accountName, const QString &roomName, const Message &m)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, roomName, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertReplaceMessage(), db);
        query.addBindValue(QString::fromLatin1(m.messageId()));
        query.addBindValue(m.timeStamp());
        // qDebug() << " m.timeStamp() " << m.timeStamp();
        // FIXME look at why we can't save a binary ?
        query.addBindValue(Message::serialize(m, false)); // TODO binary or not ?
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
        }
    }
}

void LocalMessagesDatabase::deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, roomName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteMessage(), db);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
    }
}

QString LocalMessagesDatabase::generateQueryStr(qint64 startId, qint64 endId, qint64 numberElements)
{
    QString query = u"SELECT * FROM MESSAGES"_s;
    if (startId != -1) {
        query += u" WHERE timestamp >= :startId"_s;
        if (endId != -1) {
            query += u" AND timestamp <= :endId"_s;
        }
    } else {
        if (endId != -1) {
            query += u" WHERE timestamp <= :endId"_s;
        }
    }
    query += u" ORDER BY timestamp DESC"_s;

    if (numberElements != -1) {
        query += u" LIMIT :limit"_s;
    }
    return query;
}

QList<Message>
LocalMessagesDatabase::loadMessages(RocketChatAccount *account, const QString &_roomName, qint64 startId, qint64 endId, qint64 numberElements) const
{
    Q_ASSERT(account);
    return loadMessages(account->accountName(), _roomName, startId, endId, numberElements, account->emojiManager());
}

QList<Message> LocalMessagesDatabase::loadMessages(const QString &accountName,
                                                   const QString &_roomName,
                                                   qint64 startId,
                                                   qint64 endId,
                                                   qint64 numberElements,
                                                   EmojiManager *emojiManager) const
{
#if 0
    SELECT id, nom, email
    FROM utilisateurs
    LIMIT 5 OFFSET 5;

    // Use sorting ASC or DESC
#endif

    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + u'-' + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomName);
        // qDebug() << " fileName " << fileName;
        if (!QFileInfo::exists(fileName)) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Filename doesn't exist: " << fileName;
            return {};
        }
        db = QSqlDatabase::addDatabase(u"QSQLITE"_s, dbName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't open" << fileName;
            return {};
        }
    }

    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    const QString query = LocalMessagesDatabase::generateQueryStr(startId, endId, numberElements);
    QSqlQuery resultQuery(db);
    resultQuery.prepare(query);
    if (startId != -1) {
        resultQuery.bindValue(u":startId"_s, startId);
        if (endId != -1) {
            resultQuery.bindValue(u":endId"_s, endId);
        }
    } else {
        if (endId != -1) {
            resultQuery.bindValue(u":endId"_s, endId);
        }
    }
    if (numberElements != -1) {
        resultQuery.bindValue(u":limit"_s, numberElements);
    }
    if (!resultQuery.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << " Impossible to execute query: " << resultQuery.lastError() << " query: " << query;
        return {};
    }

    QList<Message> listMessages;
    while (resultQuery.next()) {
        const QString json = resultQuery.value(u"json"_s).toString();
        listMessages.append(convertJsonToMessage(json, emojiManager));
    }
    return listMessages;
}

Message LocalMessagesDatabase::convertJsonToMessage(const QString &json, EmojiManager *emojiManager)
{
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    const Message msg = Message::deserialize(doc.object(), emojiManager);
    return msg;
}

std::unique_ptr<QSqlTableModel> LocalMessagesDatabase::createMessageModel(const QString &accountName, const QString &_roomName) const
{
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + u'-' + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomName);
        // qDebug() << " fileName " << fileName;
        if (!QFileInfo::exists(fileName)) {
            return {};
        }
        db = QSqlDatabase::addDatabase(u"QSQLITE"_s, dbName);
        db.setDatabaseName(fileName);
        if (!db.open()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't open" << fileName;
            return {};
        }
    }

    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    auto model = std::make_unique<QSqlTableModel>(nullptr, db);
    model->setTable(u"MESSAGES"_s);
    model->setSort(int(MessagesFields::TimeStamp), Qt::AscendingOrder);
    model->select();
    return model;
}
