/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "localdatabasebase.h"

class LocalRoomSubscriptionsDatabase : public LocalDatabaseBase
{
public:
    LocalRoomSubscriptionsDatabase();
    ~LocalRoomSubscriptionsDatabase() override;
};
