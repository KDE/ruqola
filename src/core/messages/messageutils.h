/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QByteArray>
namespace MessageUtils
{
[[nodiscard]] QByteArray generateUniqueId(const QByteArray &messageId, int index);
};
