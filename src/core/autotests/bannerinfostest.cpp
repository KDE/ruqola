/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfostest.h"
#include "bannerinfo/bannerinfos.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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

    QTest::addRow("bannerinfos-empty") << u"bannerinfos-empty"_s << 0 << false;
    QTest::addRow("bannerinfos1") << u"bannerinfos1"_s << 3 << false;
    QTest::addRow("bannerinfos2") << u"bannerinfos2"_s << 3 << true;
    QTest::addRow("bannerinfos3") << u"bannerinfos3"_s << 3 << true;
}

void BannerInfosTest::shouldLoadBannerInfos()
{
    QFETCH(QString, name);
    QFETCH(int, bannersCount);
    QFETCH(bool, hasUnreadBanner);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/bannerinfos/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    BannerInfos r;
    r.parseBannerInfos(obj);
    QCOMPARE(r.count(), bannersCount);
    QCOMPARE(r.bannerUnreadInformations().isEmpty(), !hasUnreadBanner);
}

#include "moc_bannerinfostest.cpp"
