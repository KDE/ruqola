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
