/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "paginatedinfolist.h"

#include "ruqola_debug.h"

#include <QJsonValue>

void PaginatedInfoListPrivate::warnInvalidIndex(int index)
{
    qCWarning(RUQOLA_LOG) << "Invalid index " << index;
}

void PaginatedInfoListPrivate::warnInvalidElement(QLatin1StringView arrayKey, const QJsonValue &value)
{
    qCWarning(RUQOLA_LOG) << "Problem when parsing" << arrayKey << value;
}
