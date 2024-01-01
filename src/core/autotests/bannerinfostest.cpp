/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfostest.h"
#include "bannerinfo/bannerinfos.h"
#include "ruqola_autotest_helper.h"

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
    QVERIFY(w.bannerUnreadInformations().isEmpty());
}

void BannerInfosTest::shouldLoadBannerInfos_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("bannersCount");
    QTest::addColumn<bool>("hasUnreadBanner");

    QTest::addRow("bannerinfos-empty") << QStringLiteral("bannerinfos-empty") << 0 << false;
    QTest::addRow("bannerinfos1") << QStringLiteral("bannerinfos1") << 3 << false;
    QTest::addRow("bannerinfos2") << QStringLiteral("bannerinfos2") << 3 << true;
    QTest::addRow("bannerinfos3") << QStringLiteral("bannerinfos3") << 3 << true;
}

void BannerInfosTest::shouldLoadBannerInfos()
{
    QFETCH(QString, name);
    QFETCH(int, bannersCount);
    QFETCH(bool, hasUnreadBanner);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/bannerinfos/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    BannerInfos r;
    r.parseBannerInfos(obj);
    QCOMPARE(r.count(), bannersCount);
    QCOMPARE(r.bannerUnreadInformations().isEmpty(), !hasUnreadBanner);
}

#include "moc_bannerinfostest.cpp"
