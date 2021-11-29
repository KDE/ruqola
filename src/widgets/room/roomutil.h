/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QString>
namespace RoomUtil
{
LIBRUQOLAWIDGETS_TESTS_EXPORT Q_REQUIRED_RESULT QString generateUserLink(const QString &userName);
LIBRUQOLAWIDGETS_TESTS_EXPORT Q_REQUIRED_RESULT QString generatePermalink(const QString &messageId, const QString &roomId, Room::RoomType channelType);
}

