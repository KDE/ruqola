/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountsdatabase.h"
using namespace Qt::Literals::StringLiterals;

#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

static const char s_schemaAccountsDataBase[] = "CREATE TABLE ACCOUNT (accountName TEXT PRIMARY KEY NOT NULL, json TEXT)";
enum class AccountFields {
    AccountName,
    Json,
}; // in the same order as the table

LocalAccountsDatabase::LocalAccountsDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localAccountsDatabasePath(), LocalDatabaseBase::DatabaseType::Accounts)
{
}

LocalAccountsDatabase::~LocalAccountsDatabase() = default;

QString LocalAccountsDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaAccountsDataBase);
}

void LocalAccountsDatabase::updateAccount(const QString &accountName, const QByteArray &ba)
{
    QSqlDatabase db;
    if (initializeDataBase(accountName, db)) {
        QSqlQuery query(LocalDatabaseUtils::updateAccount(), db);
        query.addBindValue(accountName);
        query.addBindValue(ba);
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in ACCOUNT table" << db.databaseName() << query.lastError();
        }
    }
}

void LocalAccountsDatabase::deleteAccount(const QString &accountName)
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

QByteArray LocalAccountsDatabase::jsonAccount(const QString &accountName)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return {};
    }
    QSqlQuery query(LocalDatabaseUtils::jsonAccount().arg(accountName), db);
    QByteArray value;
    // We have one element
    if (query.first()) {
        value = query.value(0).toByteArray();
    }
    return value;
}
