/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "globaldatabase.h"

#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"

#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
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

QString GlobalDatabase::generateIdentifier(const QString &accountName, const QByteArray &roomId, TimeStampType type)
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
    case TimeStampType::UpdateGlobalRoomsTimeStamp:
        identifier = u"update-global-rooms-"_s;
        break;
    }
    identifier += accountName;
    if (roomId.isEmpty() && (type != TimeStampType::AccountTimeStamp && type != TimeStampType::UpdateGlobalRoomsTimeStamp)) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Missing roomName! It's a bug!!!";
    }
    if (!roomId.isEmpty()) {
        identifier += u'-' + QString::fromLatin1(roomId);
    }
    return identifier;
}

void GlobalDatabase::insertOrReplaceTimeStamp(const QString &accountName, const QByteArray &roomId, qint64 timestamp, TimeStampType type)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        const QString identifier = generateIdentifier(accountName, roomId, type);
        QSqlQuery query(LocalDatabaseUtils::insertReplaceGlobal(), db);
        query.addBindValue(identifier);
        query.addBindValue(timestamp);
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
        }
    }
}

void GlobalDatabase::removeTimeStamp(const QString &accountName, const QByteArray &roomId, TimeStampType type)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    const QString identifier = generateIdentifier(accountName, roomId, type);
    QSqlQuery query(LocalDatabaseUtils::removeGlobal(), db);
    query.addBindValue(identifier);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in GLOBAL table" << db.databaseName() << query.lastError();
    }
}

qint64 GlobalDatabase::timeStamp(const QString &accountName, const QByteArray &roomId, TimeStampType type)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return -1;
    }
    const QString identifier = generateIdentifier(accountName, roomId, type);
    QSqlQuery query(LocalDatabaseUtils::timestampGlobal().arg(identifier), db);
    qint64 value = -1;
    // We have one element
    if (query.first()) {
        value = query.value(0).toLongLong();
    }
    return value;
}

std::unique_ptr<QSqlTableModel> GlobalDatabase::createGlobalModel(const QString &accountName) const
{
    const QString dbName = databaseName(accountName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName);
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
    model->setTable(u"GLOBAL"_s);
    model->setSort(int(GlobalFields::TimeStamp), Qt::AscendingOrder);
    model->select();
    return model;
}
