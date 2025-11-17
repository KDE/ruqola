/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localroompendingtypedinfodatabase.h"
#include "localdatabaseutils.h"

LocalRoomPendingTypedInfoDatabase::LocalRoomPendingTypedInfoDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localMessagesDatabasePath(), LocalDatabaseBase::DatabaseType::PendingTypedInfo)
{
}

LocalRoomPendingTypedInfoDatabase::~LocalRoomPendingTypedInfoDatabase() = default;
