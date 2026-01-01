/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "room.h"
#include <QString>
#include <QWidget>
namespace RoomUtil
{
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QString generateUserLink(const QString &userName);
[[nodiscard]] LIBRUQOLAWIDGETS_TESTS_EXPORT QString generatePermalink(const QString &messageId, const QString &roomId, Room::RoomType channelType);
void positionPopup(QPoint pos, QWidget *parentWindow, QWidget *popup);
}
