/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QString>
namespace RoomUtil
{
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QString generateUserLink(const QString &userName);
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QString generatePermalink(const QString &messageId, const QString &roomId, Room::RoomType channelType);
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT bool validUser(const QString &userName);
}
