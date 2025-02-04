/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loadrecenthistorymanager.h"
#include "ruqola_debug.h"

LoadRecentHistoryManager::LoadRecentHistoryManager() = default;

LoadRecentHistoryManager::~LoadRecentHistoryManager() = default;

qint64 LoadRecentHistoryManager::lastLoadingTimeStamp() const
{
    return mLastLoadingTimeStamp;
}

qint64 LoadRecentHistoryManager::generateNewStartTimeStamp(qint64 lastTimeStamp)
{
    constexpr qint64 result = 86400 * 3 * 1000;
    const qint64 newTimeStamp = qMin(mLastLoadingTimeStamp, lastTimeStamp) - result;
    mLastLoadingTimeStamp = newTimeStamp;
    qCDebug(RUQOLA_LOG) << "newTimeStamp " << QDateTime::fromMSecsSinceEpoch(newTimeStamp);
    return newTimeStamp;
}
