/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QByteArray>
#include <QDebug>
namespace MessageUtils
{
[[nodiscard]] QByteArray generateUniqueId(const QByteArray &messageId, int index);
};
