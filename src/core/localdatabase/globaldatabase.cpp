/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "globaldatabase.h"

#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

using namespace Qt::Literals::StringLiterals;
static const char s_schemaGlobalDataBase[] = "CREATE TABLE GLOBAL (identifier TEXT PRIMARY KEY NOT NULL, timestamp INTEGER)";
enum class GlobalFields {
    Identifier,
    TimeStamp,
}; // in the same order as the table

GlobalDatabase::GlobalDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localGlobalDatabasePath(), LocalDatabaseBase::DatabaseType::Global)
{
}

GlobalDatabase::~GlobalDatabase() = default;

QString GlobalDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaGlobalDataBase);
}

QString GlobalDatabase::generateIdentifier(const QString &accountName, const QString &roomName, TimeStampType type)
{
    QString identifier;
    if (accountName.isEmpty()) {
        return identifier;
    }
    switch (type) {
    case TimeStampType::MessageTimeStamp:
        identifier = u"messages-"_s;
        break;
    case TimeStampType::RoomTimeStamp:
        identifier = u"rooms-"_s;
        break;
    case TimeStampType::AccountTimeStamp:
        identifier = u"account-"_s;
        break;
    }
    identifier += accountName;
    if (roomName.isEmpty() && type != TimeStampType::AccountTimeStamp) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Missing roomName! It's a bug!!!";
    }
    if (!roomName.isEmpty()) {
        identifier += u'-' + LocalDatabaseUtils::fixRoomName(roomName);
    }
    return identifier;
}

void GlobalDatabase::insertOrReplaceTimeStamp(const QString &accountName, const QString &roomName, qint64 timestamp, TimeStampType type)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        const QString identifier = generateIdentifier(accountName, roomName, type);
        QSqlQuery query(LocalDatabaseUtils::insertReplaceGlobal(), db);
        query.addBindValue(identifier);
        query.addBindValue(timestamp);
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
        }
    }
}

void GlobalDatabase::removeTimeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    const QString identifier = generateIdentifier(accountName, roomName, type);
    QSqlQuery query(LocalDatabaseUtils::removeGlobal(), db);
    query.addBindValue(identifier);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
    }
}

qint64 GlobalDatabase::timeStamp(const QString &accountName, const QString &roomName, TimeStampType type)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return -1;
    }
    const QString identifier = generateIdentifier(accountName, roomName, type);
    QSqlQuery query(LocalDatabaseUtils::timestampGlobal().arg(identifier), db);
    qint64 value = -1;
    // We have one element
    if (query.first()) {
        value = query.value(0).toLongLong();
    }
    return value;
}

std::unique_ptr<QSqlTableModel> GlobalDatabase::createGlobalModel() const
{
    /*
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
    */
    return nullptr;
}
