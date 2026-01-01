/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "actionbutton.h"
#include "libruqolacore_export.h"
#include "room.h"
namespace ActionButtonUtil
{
[[nodiscard]] LIBRUQOLACORE_EXPORT ActionButton::RoomTypeFilter convertRoomTypeToActionButtonRoomTypeFilter(Room *r);
[[nodiscard]] LIBRUQOLACORE_EXPORT QString generateTranslateIdentifier(const ActionButton &actionButton);
};
