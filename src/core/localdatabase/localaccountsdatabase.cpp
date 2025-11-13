/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localaccountsdatabase.h"

#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

using namespace Qt::Literals::StringLiterals;
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
        } else if (mRuqolaLogger) {
            mRuqolaLogger->dataSaveFromDatabase("Update account " + accountName.toUtf8());
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
    } else if (mRuqolaLogger) {
        mRuqolaLogger->dataSaveFromDatabase("Delete account " + accountName.toUtf8());
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

std::unique_ptr<QSqlTableModel> LocalAccountsDatabase::createAccountsModel(const QString &accountName) const
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
    model->setTable(u"ACCOUNT"_s);
    model->setSort(int(AccountFields::AccountName), Qt::AscendingOrder);
    model->select();
    return model;
}
