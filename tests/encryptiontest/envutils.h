/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QHash>
#include <QString>

[[nodiscard]] QHash<QString, QString> loadEnvFile(const QString &filePath);
