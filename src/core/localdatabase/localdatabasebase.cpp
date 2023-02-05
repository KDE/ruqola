/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebase.h"
#include "ruqola_database_debug.h"

LocalDatabaseBase::LocalDatabaseBase(const QString &basePath, LocalDatabaseBase::DatabaseType type)
    : mBasePath(basePath)
    , mDatabaseType(type)
{
}

LocalDatabaseBase::~LocalDatabaseBase() = default;

QString LocalDatabaseBase::dbFileName(const QString &accountName, const QString &roomName) const
{
    const QString dirPath = mBasePath + accountName;
    return dirPath + QLatin1Char('/') + roomName + QStringLiteral(".sqlite");
}

QString LocalDatabaseBase::schemaDataBase() const
{
    Q_ASSERT(true);
    return {};
}

QString LocalDatabaseBase::databaseName(const QString &name) const
{
    QString prefix;
    switch (mDatabaseType) {
    case DatabaseType::Unknown:
        qCWarning(RUQOLA_DATABASE_LOG) << "Unknown data base it's a bug";
        break;
    case DatabaseType::Account:
        prefix = QStringLiteral("accounts-");
    case DatabaseType::Rooms:
        prefix = QStringLiteral("rooms-");
        break;
    case DatabaseType::Message:
        prefix = QStringLiteral("messages-");
        break;
    case DatabaseType::Logger:
        break;
    }

    return prefix + name;
}
