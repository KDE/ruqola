/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebase.h"
#include "ruqola_database_debug.h"

LocalDatabaseBase::LocalDatabaseBase(const QString &basePath)
    : mBasePath(basePath)
{
}

LocalDatabaseBase::~LocalDatabaseBase() = default;

QString LocalDatabaseBase::dbFileName(const QString &accountName, const QString &roomName) const
{
    const QString dirPath = mBasePath + accountName;
    return dirPath + QLatin1Char('/') + roomName + QStringLiteral(".sqlite");
}

QString LocalDatabaseBase::databaseNamePrefix(const QString &name)
{
    QString prefix;
    switch (mDatabaseType) {
    case DatabaseType::Unknown:
        qCWarning(RUQOLA_DATABASE_LOG) << "Unknown data base it's a bug";
        break;
    case DatabaseType::Account:
    case DatabaseType::Rooms:
        prefix = QStringLiteral("rooms-");
        break;
    case DatabaseType::Message:
    case DatabaseType::Logger:
        break;
    }

    return prefix;
}
