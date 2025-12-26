/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localroomsubscriptionsdatabase.h"
#include "localdatabase/localdatabaseutils.h"

LocalRoomSubscriptionsDatabase::LocalRoomSubscriptionsDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localRoomSubscriptionsDatabasePath(), LocalDatabaseBase::DatabaseType::RoomSubscriptions)
{
}

LocalRoomSubscriptionsDatabase::~LocalRoomSubscriptionsDatabase() = default;
