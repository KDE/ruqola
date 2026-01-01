/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QString>

namespace GrabScreenPluginToolUtil
{
[[nodiscard]] QString picturePath(const QString &accountName);
[[nodiscard]] QString generateFileName(const QString &dirPath);
};
