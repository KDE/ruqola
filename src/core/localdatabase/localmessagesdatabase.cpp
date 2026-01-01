/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagesdatabase.h"

#include "localdatabaseutils.h"
#include "messages/message.h"
#include "rocketchataccount.h"
#include "ruqola_database_debug.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

using namespace Qt::Literals::StringLiterals;
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

void LocalMessagesDatabase::deleteDatabaseFromRoomId(const QString &accountName, const QByteArray &roomId)
{
    const QString dbName = databaseName(accountName + u'-' + QString::fromLatin1(roomId));
    QSqlDatabase::removeDatabase(dbName);
    const QString fileName = dbFileName(accountName, roomId);
    if (!QFileInfo::exists(fileName)) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Filename doesn't exist: " << fileName;
        return;
    } else {
        if (!QFile(fileName).remove()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Impossible to remove: " << fileName;
        } else {
            qCWarning(RUQOLA_DATABASE_LOG) << fileName << " was removed";
        }
    }
}

QString LocalMessagesDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaMessagesDataBase);
}

void LocalMessagesDatabase::addMessage(const QString &accountName, const QByteArray &roomId, const Message &m)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, roomId, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertReplaceMessage(), db);
        query.addBindValue(QString::fromLatin1(m.messageId()));
        query.addBindValue(m.timeStamp());
        // qDebug() << " m.timeStamp() " << m.timeStamp();
        // FIXME look at why we can't save a binary ?
        query.addBindValue(Message::serialize(m, false)); // TODO binary or not ?
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
        } else if (mRuqolaLogger) {
            mRuqolaLogger->dataSaveFromDatabase("add message in account " + accountName.toUtf8() + " in roomName " + roomId + " for message id "
                                                + m.messageId());
        }
    }
}

void LocalMessagesDatabase::deleteMessage(const QString &accountName, const QByteArray &roomId, const QString &messageId)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, roomId, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteMessage(), db);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
    } else if (mRuqolaLogger) {
        mRuqolaLogger->dataSaveFromDatabase("delete message in " + accountName.toUtf8() + " roomName " + roomId + " message id " + messageId.toUtf8());
    }
}

QString LocalMessagesDatabase::generateQueryStr(qint64 startId, qint64 endId, qint64 numberElements)
{
    qDebug() << " startId " << QDateTime::fromMSecsSinceEpoch(startId) << "endId " << QDateTime::fromMSecsSinceEpoch(endId) << " numberOfElement "
             << numberElements;
    QString query = u"SELECT * FROM MESSAGES"_s;

    if (startId != -1) {
        query += u" WHERE timestamp >= :startId"_s;
        if (endId != -1) {
            query += u" AND timestamp <= :endId"_s;
        }
    } else if (endId != -1) {
        query += u" WHERE timestamp <= :endId"_s;
    }
    query += u" ORDER BY timestamp DESC"_s;

    if (numberElements != -1) {
        query += u" LIMIT :limit"_s;
    }

    qDebug() << " query " << query;

    return query;
}

QList<Message>
LocalMessagesDatabase::loadMessages(RocketChatAccount *account, const QByteArray &roomId, qint64 startId, qint64 endId, qint64 numberElements) const
{
    Q_ASSERT(account);
    return loadMessages(account->accountName(), roomId, startId, endId, numberElements, account->emojiManager());
}

QList<Message> LocalMessagesDatabase::loadMessages(const QString &accountName,
                                                   const QByteArray &roomId,
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

    const QString dbName = databaseName(accountName + u'-' + QString::fromLatin1(roomId));
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomId);
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
    } else if (endId != -1) {
        resultQuery.bindValue(u":endId"_s, endId);
    }
    if (numberElements != -1) {
        resultQuery.bindValue(u":limit"_s, numberElements);
    }
    qDebug() << " :startId " << QDateTime::fromMSecsSinceEpoch(startId) << " :endId" << QDateTime::fromMSecsSinceEpoch(endId);
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

std::unique_ptr<QSqlTableModel> LocalMessagesDatabase::createMessageModel(const QString &accountName, const QByteArray &roomId) const
{
    const QString dbName = databaseName(accountName + u'-' + QString::fromLatin1(roomId));
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName, roomId);
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
