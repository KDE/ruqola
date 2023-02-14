/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QString>
class QSqlDatabase;
class LIBRUQOLACORE_EXPORT LocalDatabaseBase
{
public:
    enum class DatabaseType {
        Unknown = 0,
        Account,
        Rooms,
        Message,
        Logger,
        Global,
    };
    explicit LocalDatabaseBase(const QString &basePath, DatabaseType type);
    virtual ~LocalDatabaseBase();
    Q_REQUIRED_RESULT QString dbFileName(const QString &accountName, const QString &roomName) const;
    Q_REQUIRED_RESULT QString dbFileName(const QString &accountName) const;

protected:
    Q_REQUIRED_RESULT virtual QString schemaDataBase() const;
    Q_REQUIRED_RESULT bool initializeDataBase(const QString &accountName, const QString &_roomName, QSqlDatabase &db);
    Q_REQUIRED_RESULT bool initializeDataBase(const QString &accountName, QSqlDatabase &db);
    Q_REQUIRED_RESULT bool checkDataBase(const QString &accountName, const QString &_roomName, QSqlDatabase &db);
    Q_REQUIRED_RESULT bool checkDataBase(const QString &accountName, QSqlDatabase &db);
    Q_REQUIRED_RESULT QString databaseName(const QString &name) const;
    const QString mBasePath;
    const DatabaseType mDatabaseType = DatabaseType::Unknown;
};
