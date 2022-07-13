/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfostest.h"
#include "bannerinfo/bannerinfos.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(BannerInfosTest)
BannerInfosTest::BannerInfosTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfosTest::shouldHaveDefaultValues()
{
    BannerInfos w;
    QVERIFY(w.isEmpty());
    QCOMPARE(w.count(), 0);
    QVERIFY(!w.hasUnreadBanner());
}

void BannerInfosTest::shouldLoadRoles_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("bannersCount");

    QTest::addRow("bannerinfos-empty") << QStringLiteral("bannerinfos-empty") << 0;
    QTest::addRow("bannerinfos1") << QStringLiteral("bannerinfos1") << 3;
}

void BannerInfosTest::shouldLoadRoles()
{
    QFETCH(QString, name);
    QFETCH(int, bannersCount);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/bannerinfos/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    BannerInfos r;
    r.parseBannerInfos(obj);
    QCOMPARE(r.count(), bannersCount);
}
