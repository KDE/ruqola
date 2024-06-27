/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewinfotest.h"
#include "whatsnew/whatsnewinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(WhatsNewInfoTest)
WhatsNewInfoTest::WhatsNewInfoTest(QObject *parent)
    : QObject{parent}
{
}

void WhatsNewInfoTest::shouldHaveDefaultValues()
{
    WhatsNewInfo t;
    QVERIFY(t.changes().isEmpty());
    QVERIFY(t.newFeatures().isEmpty());
    QVERIFY(t.bugFixings().isEmpty());
}

#include "moc_whatsnewinfotest.cpp"
