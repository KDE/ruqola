/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountdatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

static const char s_schemaAccountDataBase[] = "CREATE TABLE ACCOUNT (accountName TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class AccountFields {
    AccountName,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalAccountDatabase::LocalAccountDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localAccountDatabasePath(), LocalDatabaseBase::DatabaseType::Account)
{
}

LocalAccountDatabase::~LocalAccountDatabase() = default;

QString LocalAccountDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaAccountDataBase);
}

void LocalAccountDatabase::updateAccount(const QString &accountName, const QByteArray &ba, qint64 timeStamp)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        QSqlQuery query(LocalDatabaseUtils::updateAccount(), db);
        query.addBindValue(accountName);
        query.addBindValue(timeStamp);
        query.addBindValue(ba);
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ACCOUNT table" << db.databaseName() << query.lastError();
        }
    }
}

void LocalAccountDatabase::deleteAccount(const QString &accountName)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return;
    }
    QSqlQuery query(LocalDatabaseUtils::deleteAccount(), db);
    query.addBindValue(accountName);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ACCOUNT table" << db.databaseName() << query.lastError();
    }
}

qint64 LocalAccountDatabase::timeStamp(const QString &accountName)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return -1;
    }
    QSqlQuery query(QStringLiteral("SELECT timestamp FROM ACCOUNT WHERE accountName = \"%1\"").arg(accountName), db);
    qint64 value = -1;
    // We have one element
    if (query.first()) {
        value = query.value(0).toLongLong();
    }
    return value;
}

QByteArray LocalAccountDatabase::jsonAccount(const QString &accountName)
{
    QSqlDatabase db;
    if (!checkDataBase(accountName, db)) {
        return {};
    }
    QSqlQuery query(QStringLiteral("SELECT json FROM ACCOUNT WHERE accountName = \"%1\"").arg(accountName), db);
    QByteArray value;
    // We have one element
    if (query.first()) {
        value = query.value(0).toByteArray();
    }
    return value;
}
