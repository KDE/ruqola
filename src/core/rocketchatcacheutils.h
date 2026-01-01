/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqola_private_export.h"
#include "managerdatapaths.h"
#include <QString>
#include <QUrl>
namespace RocketChatCacheUtils
{
[[nodiscard]] LIBRUQOLACORE_TESTS_EXPORT QString fileCachePath(const QUrl &url,
                                                               ManagerDataPaths::PathType type,
                                                               const QString &accountName,
                                                               const QString &accountServerHost);
};
