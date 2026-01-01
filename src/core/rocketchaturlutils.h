/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QString>

namespace RocketChatUrlUtils
{
[[nodiscard]] LIBRUQOLACORE_EXPORT bool parseUrl(const QString &link);
};
