/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appscountinfotest.h"
#include "apps/appscountinfo.h"
#include <QTest>
QTEST_MAIN(AppsCountInfoTest)
AppsCountInfoTest::AppsCountInfoTest(QObject *parent)
    : QObject{parent}
{
}

void AppsCountInfoTest::shouldHaveDefaultValues()
{
    AppsCountInfo w;
    QCOMPARE(w.maxMarketplaceApps(), -1);
    QCOMPARE(w.maxPrivateApps(), -1);
    QCOMPARE(w.totalMarketplaceEnabled(), -1);
    QCOMPARE(w.totalPrivateEnabled(), -1);
}

#include "moc_appscountinfotest.cpp"
