/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "awaymanagertest.h"
#include "away/awaymanager.h"
#include <QTest>
QTEST_MAIN(AwayManagerTest)
AwayManagerTest::AwayManagerTest(QObject *parent)
    : QObject{parent}
{
}

void AwayManagerTest::shouldHaveDefaultValues()
{
    AwayManager m(nullptr);
    QVERIFY(!m.enabled());
    QCOMPARE(m.idleTiming(), -1);
    // TODO
}

#include "moc_awaymanagertest.cpp"
