/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localroomsubscriptionsdatabase.h"
#include "localdatabase/localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

using namespace Qt::Literals::StringLiterals;

static const char s_schemaRoomPSubscriptionsDataBase[] = "CREATE TABLE ROOMSUBSCRIPTIONS (subscriptionId TEXT PRIMARY KEY NOT NULL, roomId TEXT)";
enum class RoomSubscriptionFields {
    SubscriptionId,
    RoomId,
}; // in the same order as the table

LocalRoomSubscriptionsDatabase::LocalRoomSubscriptionsDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localRoomSubscriptionsDatabasePath(), LocalDatabaseBase::DatabaseType::RoomSubscriptions)
{
}

LocalRoomSubscriptionsDatabase::~LocalRoomSubscriptionsDatabase() = default;

QString LocalRoomSubscriptionsDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaRoomPSubscriptionsDataBase);
}

std::unique_ptr<QSqlTableModel> LocalRoomSubscriptionsDatabase::createRoomSubscriptionsModel(const QString &accountName) const
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
    model->setTable(u"ROOMSUBSCRIPTIONS"_s);
    model->setSort(int(RoomSubscriptionFields::SubscriptionId), Qt::AscendingOrder);
    model->select();
    return model;
}

void LocalRoomSubscriptionsDatabase::deleteRoomSubscription(const QString &accountName, const QByteArray &subscriptionId)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteRoomSubscription(), db);
    query.addBindValue(QString::fromLatin1(subscriptionId));
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMSUBSCRIPTIONS table" << db.databaseName() << query.lastError();
    } else if (mRuqolaLogger) {
        mRuqolaLogger->dataSaveFromDatabase("Delete subscriptionId " + subscriptionId + " in account " + accountName.toUtf8());
    }
}

void LocalRoomSubscriptionsDatabase::insertRoomSubscription(const QString &accountName, const QByteArray &subscriptionId, const QByteArray &roomId)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        QSqlQuery query(LocalDatabaseUtils::insertRoomSubscription(), db);
        query.addBindValue(QString::fromLatin1(subscriptionId));
        query.addBindValue(QString::fromLatin1(roomId));
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ROOMSUBSCRIPTIONS table" << db.databaseName() << query.lastError();
        } else if (mRuqolaLogger) {
            mRuqolaLogger->dataSaveFromDatabase("Update subscriptionId " + subscriptionId + " for roomId " + roomId + " in account " + accountName.toUtf8());
        }
    }
}

QByteArray LocalRoomSubscriptionsDatabase::roomId(const QString &accountName, const QByteArray &subscriptionId)
{
    /*
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
    */
    // TODO
    return {};
}
