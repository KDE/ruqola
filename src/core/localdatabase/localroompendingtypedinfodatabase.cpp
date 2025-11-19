/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localroompendingtypedinfodatabase.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
using namespace Qt::Literals::StringLiterals;

static const char s_schemaRoomPendingTypedDataBase[] = "CREATE TABLE ROOMPENDINGTYPED (roomId TEXT PRIMARY KEY NOT NULL, json TEXT)";
enum class RoomPendingTypedFields {
    RoomId,
    Json,
}; // in the same order as the table

LocalRoomPendingTypedInfoDatabase::LocalRoomPendingTypedInfoDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localRoomPendingTypedInfoDatabasePath(), LocalDatabaseBase::DatabaseType::PendingTypedInfo)
{
}

LocalRoomPendingTypedInfoDatabase::~LocalRoomPendingTypedInfoDatabase() = default;

QString LocalRoomPendingTypedInfoDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaRoomPendingTypedDataBase);
}

std::unique_ptr<QSqlTableModel> LocalRoomPendingTypedInfoDatabase::createRoomsModel(const QString &accountName) const
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
    model->setTable(u"ROOMPENDINGTYPED"_s);
    model->setSort(int(RoomPendingTypedFields::RoomId), Qt::AscendingOrder);
    model->select();
    return model;
}

void LocalRoomPendingTypedInfoDatabase::updateRoomPendingTypedInfo(const QString &accountName,
                                                                   const QByteArray &roomId,
                                                                   const AccountRoomSettings::PendingTypedInfo &room)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertReplaceRoomPendingTypedInfo(), db);
        query.addBindValue(QString::fromLatin1(roomId));
        query.addBindValue(QJsonDocument(AccountRoomSettings::PendingTypedInfo::serialize(room)).toJson(QJsonDocument::Compact));
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMPENDINGTYPED table" << db.databaseName() << query.lastError();
        } else if (mRuqolaLogger) {
            mRuqolaLogger->dataSaveFromDatabase("Update roomPendingTypedInfo roomId " + roomId + " in account " + accountName.toUtf8());
        }
    }
}

void LocalRoomPendingTypedInfoDatabase::deleteRoomPendingTypedInfo(const QString &accountName, const QByteArray &roomId)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteRoomPendingTypedInfo(), db);
    query.addBindValue(QString::fromLatin1(roomId));
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMPENDINGTYPED table" << db.databaseName() << query.lastError();
    } else if (mRuqolaLogger) {
        mRuqolaLogger->dataSaveFromDatabase("Delete roomPendingTypedInfo " + roomId + " in account " + accountName.toUtf8());
    }
}

QMap<QByteArray /*RoomId*/, AccountRoomSettings::PendingTypedInfo> LocalRoomPendingTypedInfoDatabase::loadRoomPendingTypedInfo(const QString &accountName)
{
    QMap<QByteArray /*RoomId*/, AccountRoomSettings::PendingTypedInfo> info;
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Could not initialize database from " << accountName << " filename : " << dbFileName(accountName);
        return info;
    }
    qDebug() << " const QString fileName = dbFileName(accountName);" << dbFileName(accountName);
    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());

    const QString query = u"SELECT * FROM ROOMPENDINGTYPED"_s;
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
}

AccountRoomSettings::PendingTypedInfo LocalRoomPendingTypedInfoDatabase::convertJsonToRoomPendingTypedInfo(const QString &json)
{
    const QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    const AccountRoomSettings::PendingTypedInfo msg = AccountRoomSettings::PendingTypedInfo::deserialize(doc.object());
    return msg;
}
