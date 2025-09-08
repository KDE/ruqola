/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "e2edatabase.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QSqlError>
#include <QSqlQuery>

const char E2EDataBase::s_schemaE2EKeyStore[] = "CREATE TABLE E2EKEYS (userId TEXT PRIMARY KEY NOT NULL, encryptedPrivateKey BLOB, publicKey BLOB)";

E2EDataBase::E2EDataBase()
    : LocalDatabaseBase(LocalDatabaseUtils::localDatabasePath() + QStringLiteral("e2e/"), LocalDatabaseBase::DatabaseType::E2E)
{
}

E2EDataBase::~E2EDataBase() = default;

QString E2EDataBase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaE2EKeyStore);
}

bool E2EDataBase::saveKey(const QString &userId, const QByteArray &encryptedPrivateKey, const QByteArray &publicKey)
{
    QSqlDatabase db;
    if (!initializeDataBase(userId, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT OR REPLACE INTO E2EKEYS (userId, encryptedPrivateKey, publicKey) VALUES (?, ?, ?)"));
    query.addBindValue(userId);
    query.addBindValue(encryptedPrivateKey);
    query.addBindValue(publicKey);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in E2EKEYS table" << db.databaseName() << query.lastError();
        return false;
    }
    return true;
}

bool E2EDataBase::loadKey(const QString &userId, QByteArray &encryptedPrivateKey, QByteArray &publicKey)
{
    QSqlDatabase db;
    if (!initializeDataBase(userId, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT encryptedPrivateKey, publicKey FROM E2EKEYS WHERE userId = ?"));
    query.addBindValue(userId);
    if (query.exec() && query.first()) {
        encryptedPrivateKey = query.value(0).toByteArray();
        publicKey = query.value(1).toByteArray();
        return true;
    }
    return false;
}

bool E2EDataBase::deleteKey(const QString &userId)
{
    QSqlDatabase db;
    if (!initializeDataBase(userId, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM E2EKEYS WHERE userId = ?"));
    query.addBindValue(userId);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't delete from E2EKEYS table" << db.databaseName() << query.lastError();
        return false;
    }
    return true;
}

bool E2EDataBase::hasKey(const QString &userId)
{
    QSqlDatabase db;
    if (!initializeDataBase(userId, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT 1 FROM E2EKEYS WHERE userId = ?"));
    query.addBindValue(userId);
    return query.exec() && query.first();
}
