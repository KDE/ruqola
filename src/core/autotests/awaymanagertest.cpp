/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
