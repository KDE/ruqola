/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "loadrecenthistorymanagertest.h"
#include "loadrecenthistorymanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(LoadRecentHistoryManagerTest)

LoadRecentHistoryManagerTest::LoadRecentHistoryManagerTest(QObject *parent)
    : QObject(parent)
{
}

void LoadRecentHistoryManagerTest::shouldIncreaseTimer()
{
    LoadRecentHistoryManager manager;
    const qint64 lastLoadingTimeStamp = manager.lastLoadingTimeStamp();
    const qint64 newTimeStamp = manager.generateNewStartTimeStamp(lastLoadingTimeStamp);
    QCOMPARE(newTimeStamp, lastLoadingTimeStamp - (86400 * 3 * 1000));
}

#include "moc_loadrecenthistorymanagertest.cpp"
