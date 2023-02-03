/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localdatabasebase.h"

#pragma once

class LocalRoomsDatabase : public LocalDatabaseBase
{
public:
    LocalRoomsDatabase();
    ~LocalRoomsDatabase();
};
