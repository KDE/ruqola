/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractlogger.h"
#include "libruqolacore_export.h"
#include <QString>
class QSqlDatabase;
class LIBRUQOLACORE_EXPORT LocalDatabaseBase
{
public:
    enum class DatabaseType {
        Unknown = 0,
        Accounts,
        Rooms,
        Messages,
        Logger,
        Global,
    };
    explicit LocalDatabaseBase(const QString &basePath, DatabaseType type);
    virtual ~LocalDatabaseBase();
    [[nodiscard]] QString dbFileName(const QString &accountName, const QString &roomName) const;
    [[nodiscard]] QString dbFileName(const QString &accountName, const QByteArray &roomId) const;
    [[nodiscard]] QString dbFileName(const QString &accountName) const;

    // Only for test!
    [[nodiscard]] QString schemaDatabaseStr() const;
    void setDatabaseLogger(RocketChatRestApi::AbstractLogger *logger);

protected:
    [[nodiscard]] virtual QString schemaDataBase() const;
    [[nodiscard]] bool initializeDataBase(const QString &accountName, const QByteArray &roomId, QSqlDatabase &db);
    [[nodiscard]] bool initializeDataBase(const QString &accountName, QSqlDatabase &db);
    [[nodiscard]] bool checkDataBase(const QString &accountName, const QByteArray &roomId, QSqlDatabase &db);
    [[nodiscard]] bool checkDataBase(const QString &accountName, QSqlDatabase &db);
    [[nodiscard]] QString databaseName(const QString &name) const;

    const QString mBasePath;
    const DatabaseType mDatabaseType = DatabaseType::Unknown;
    RocketChatRestApi::AbstractLogger *mRuqolaLogger = nullptr;

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString generateDatabaseName(const QString &accountName, const QByteArray &roomId) const;
};
