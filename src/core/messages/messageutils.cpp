/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageutils.h"

QByteArray MessageUtils::generateUniqueId(const QByteArray &messageId, int index)
{
    QByteArray result = messageId;
    result.append('_');
    result.append(QByteArray::number(index));
    return result;
}
