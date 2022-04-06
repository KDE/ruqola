/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorymanagertest.h"
#include "notificationhistorymanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(NotificationHistoryManagerTest)

NotificationHistoryManagerTest::NotificationHistoryManagerTest(QObject *parent)
    : QObject{parent}
{
}

void NotificationHistoryManagerTest::shouldHaveDefaultValues()
{
    NotificationHistoryManager w;
    // TODO
}
