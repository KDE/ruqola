/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "e2eroomsdatabase.h"
#include "localdatabaseutils.h"
#include "ruqola_database_debug.h"
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
using namespace Qt::Literals::StringLiterals;
static const char s_schemaE2ERoomsKeysStore[] = "CREATE TABLE E2EROOMSKEYS (roomKeyId TEXT PRIMARY KEY NOT NULL, encryptedPrivateKey BLOB, publicKey BLOB)";
enum class E2ERoomsFields {
    RoomKeyId,
    EncryptedPrivateKey,
    PublicKey
}; // in the same order as the table

E2ERoomsDataBase::E2ERoomsDataBase()
    : LocalDatabaseBase(LocalDatabaseUtils::localE2ERoomsDatabasePath(), LocalDatabaseBase::DatabaseType::E2ERooms)
{
}

E2ERoomsDataBase::~E2ERoomsDataBase() = default;

QString E2ERoomsDataBase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaE2ERoomsKeysStore);
}

QString E2ERoomsDataBase::generateRoomKeyId(const QString &roomId, const QString &keyId)
{
    if (roomId.isEmpty() || keyId.isEmpty()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Impossible to generate identifier";
        return {};
    }
    return u"%1-%2"_s.arg(roomId, keyId);
}

bool E2ERoomsDataBase::saveKey(const QString &accountName,
                               const QString &roomId,
                               const QString &keyId,
                               const QByteArray &encryptedPrivateKey,
                               const QByteArray &publicKey)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT OR REPLACE INTO E2EROOMSKEYS (roomKeyId, encryptedPrivateKey, publicKey) VALUES (?, ?, ?)"));
    query.addBindValue(generateRoomKeyId(roomId, keyId));
    query.addBindValue(encryptedPrivateKey);
    query.addBindValue(publicKey);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in E2EROOMSKEYS table" << db.databaseName() << query.lastError();
        return false;
    }
    return true;
}

bool E2ERoomsDataBase::loadKey(const QString &accountName, const QString &roomId, const QString &keyId, QByteArray &encryptedPrivateKey, QByteArray &publicKey)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT encryptedPrivateKey, publicKey FROM E2EROOMSKEYS WHERE roomKeyId = ?"));
    query.addBindValue(generateRoomKeyId(roomId, keyId));
    if (query.exec() && query.first()) {
        encryptedPrivateKey = query.value(0).toByteArray();
        publicKey = query.value(1).toByteArray();
        return true;
    }
    return false;
}

bool E2ERoomsDataBase::deleteKey(const QString &accountName, const QString &roomId, const QString &keyId)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("DELETE FROM E2EROOMSKEYS WHERE roomKeyId = ?"));
    query.addBindValue(generateRoomKeyId(roomId, keyId));
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't delete from E2EROOMSKEYS table" << db.databaseName() << query.lastError();
        return false;
    }
    return true;
}

bool E2ERoomsDataBase::deleteAllKeys(const QString &accountName)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return false;
    }
    QSqlQuery query(db);
    if (!query.exec(QStringLiteral("DELETE FROM E2EROOMSKEYS"))) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't delete all keys from E2EROOMSKEYS table" << db.databaseName() << query.lastError();
        return false;
    }
    return true;
}

bool E2ERoomsDataBase::hasKey(const QString &accountName, const QString &roomId, const QString &keyId)
{
    QSqlDatabase db;
    if (!initializeDataBase(accountName, db)) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare(QStringLiteral("SELECT 1 FROM E2EROOMSKEYS WHERE roomKeyId = ?"));
    query.addBindValue(generateRoomKeyId(roomId, keyId));
    return query.exec() && query.first();
}

std::unique_ptr<QSqlTableModel> E2ERoomsDataBase::createE2eRoomsModel(const QString &accountName) const
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
    model->setTable(u"E2EROOMSKEYS"_s);
    model->setSort(int(E2ERoomsFields::RoomKeyId), Qt::AscendingOrder);
    model->select();
    return model;
}
