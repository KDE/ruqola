/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomextra.h"
#include "ruqola_room_memory_debug.h"

RoomExtra::RoomExtra()
    : QSharedData()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomExtra created " << this;
}

RoomExtra::~RoomExtra()
{
    qCDebug(RUQOLA_ROOM_MEMORY_LOG) << " RoomExtra deleted " << this;
}
