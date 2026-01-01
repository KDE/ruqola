/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "activitiesmanagertest.h"
#include "activities/activitiesmanager.h"
#include <QStandardPaths>
#include <QTest>
QTEST_GUILESS_MAIN(ActivitiesManagerTest)
ActivitiesManagerTest::ActivitiesManagerTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ActivitiesManagerTest::shouldHaveDefaultValues()
{
    ActivitiesManager w;
    QVERIFY(!w.enabled());
}

#include "moc_activitiesmanagertest.cpp"
