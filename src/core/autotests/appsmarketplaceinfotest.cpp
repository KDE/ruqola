/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "appsmarketplaceinfotest.h"
#include "apps/appsmarketplaceinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(AppsMarketPlaceInfoTest)
AppsMarketPlaceInfoTest::AppsMarketPlaceInfoTest(QObject *parent)
    : QObject{parent}
{
}

void AppsMarketPlaceInfoTest::shouldHaveDefaultValues()
{
    AppsMarketPlaceInfo d;
    QVERIFY(d.appId().isEmpty());
    QVERIFY(d.appName().isEmpty());
    QVERIFY(!d.isEnterpriseOnly());
    QVERIFY(d.categories().isEmpty());
    QVERIFY(d.documentationUrl().isEmpty());
    QVERIFY(d.purchaseType().isEmpty());
    QVERIFY(d.version().isEmpty());
    QVERIFY(d.shortDescription().isEmpty());
    QCOMPARE(d.price(), 0);
    QVERIFY(!d.isValid());
}

#include "moc_appsmarketplaceinfotest.cpp"
