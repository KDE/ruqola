/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageutils.h"

QByteArray MessageUtils::generateUniqueId(const QByteArray &messageId, int index)
{
    return messageId + QByteArray("_") + QByteArray::number(index);
}
