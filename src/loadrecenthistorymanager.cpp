/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "loadrecenthistorymanager.h"
#include "ruqola_debug.h"

LoadRecentHistoryManager::LoadRecentHistoryManager()
{
}

LoadRecentHistoryManager::~LoadRecentHistoryManager()
{
}

qint64 LoadRecentHistoryManager::lastLoadingTimeStamp() const
{
    return mLastLoadingTimeStamp;
}

qint64 LoadRecentHistoryManager::generateNewStartTimeStamp(qint64 lastTimeStamp)
{
    qint64 newTimeStamp = 0;
    if (mLastLoadingTimeStamp < lastTimeStamp) {
        newTimeStamp = mLastLoadingTimeStamp - (86400 * 3 * 1000);
    } else {
        newTimeStamp = lastTimeStamp - (86400 * 3 * 1000);
    }
    mLastLoadingTimeStamp = newTimeStamp;
    qCDebug(RUQOLA_LOG) << "newTimeStamp " << QDateTime::fromMSecsSinceEpoch(newTimeStamp);
    return newTimeStamp;
}
