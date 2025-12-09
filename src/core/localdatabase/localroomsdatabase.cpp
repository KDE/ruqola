/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localroomsdatabase.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabaseutils.h"
#include "room.h"
#include "ruqola_database_debug.h"

#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

static const char s_schemaRoomDataBase[] = "CREATE TABLE ROOMS (roomId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class RoomFields {
    RoomId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalRoomsDatabase::LocalRoomsDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localRoomsDatabasePath(), LocalDatabaseBase::DatabaseType::Rooms)
{
}

LocalRoomsDatabase::~LocalRoomsDatabase() = default;

QString LocalRoomsDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaRoomDataBase);
}

void LocalRoomsDatabase::updateRoom(const QString &accountName, Room *room)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertReplaceRoom(), db);
        query.addBindValue(QString::fromLatin1(room->roomId()));
        query.addBindValue(room->updatedAt()); // TODO ?
        query.addBindValue(Room::serialize(room, false)); // TODO use binary ?
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMS table" << db.databaseName() << query.lastError();
        } else if (mRuqolaLogger) {
            mRuqolaLogger->dataSaveFromDatabase("Update room " + room->displayRoomName().toUtf8() + " in account " + accountName.toUtf8());
        }
    }
}

void LocalRoomsDatabase::deleteRoom(const QString &accountName, const QByteArray &roomId)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteRoom(), db);
    query.addBindValue(QString::fromLatin1(roomId));
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMS table" << db.databaseName() << query.lastError();
    } else if (mRuqolaLogger) {
        mRuqolaLogger->dataSaveFromDatabase("Delete room " + roomId + " in account " + accountName.toUtf8());
    }
}

QByteArray LocalRoomsDatabase::jsonRoom(const QString &accountName, const QString &roomId)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Could not initialize database from " << accountName << " roomId " << roomId;
        return {};
    }
    QSqlQuery query(LocalDatabaseUtils::jsonRoom().arg(roomId), db);
    QByteArray value;
    // We have one element
    if (query.first()) {
        value = query.value(0).toByteArray();
    }
    return value;
}

std::unique_ptr<QSqlTableModel> LocalRoomsDatabase::createRoomsModel(const QString &accountName) const
{
    const QString dbName = databaseName(accountName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        // Open the DB if it exists (don't create a new one)
        const QString fileName = dbFileName(accountName);
        qCDebug(RUQOLA_DATABASE_LOG) << " fileName " << fileName;
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
    model->setTable(u"ROOMS"_s);
    model->setSort(int(RoomFields::TimeStamp), Qt::AscendingOrder);
    model->select();
    return model;
}

// TODO load all rooms at beginning
void LocalRoomsDatabase::loadRooms(const QString &accountName) const
{
#if 0
    QMap<QByteArray /*RoomId*/, AccountRoomSettings::PendingTypedInfo> info;
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Could not initialize database from " << accountName << " filename : " << dbFileName(accountName);
        return info;
    }
    // qDebug() << " const QString fileName = dbFileName(accountName);" << dbFileName(accountName);
    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());

    const QString query = u"SELECT * FROM ROOMS"_s;
    QSqlQuery resultQuery(db);
    resultQuery.prepare(query);
    if (!resultQuery.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << " Impossible to execute query: " << resultQuery.lastError() << " query: " << query;
        return info;
    }

    while (resultQuery.next()) {
        const QString json = resultQuery.value(u"json"_s).toString();
        const QByteArray roomId = resultQuery.value(u"roomId"_s).toByteArray();
        info.insert(roomId, convertJsonToRoomPendingTypedInfo(json));
    }
    return info;
#endif
    // TODO
}
