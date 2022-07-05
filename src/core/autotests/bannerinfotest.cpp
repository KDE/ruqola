/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannerinfotest.h"
#include "bannerinfo/bannerinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(BannerInfoTest)
BannerInfoTest::BannerInfoTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoTest::shouldHaveDefaultValues()
{
    BannerInfo w;
    QVERIFY(!w.read());
    QVERIFY(w.text().isEmpty());
    QVERIFY(w.title().isEmpty());
    QVERIFY(w.link().isEmpty());
    QVERIFY(w.textArguments().isEmpty());
    QVERIFY(w.identifier().isEmpty());
    QCOMPARE(w.priority(), -1);
}

void BannerInfoTest::shouldBannerInfo()
{
    QFETCH(QString, name);
    QFETCH(BannerInfo, bannerInfo);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/bannerinfo/") + name + QLatin1String(".json");
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);

    BannerInfo result;
    result.parseBannerInfo(obj);
    const bool equal = result == bannerInfo;
    if (!equal) {
        qDebug() << " result " << result;
        qDebug() << " bannerInfo " << bannerInfo;
    }
    QVERIFY(equal);
}

void BannerInfoTest::shouldBannerInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<BannerInfo>("bannerInfo");
    QTest::addRow("bannerinfoempty") << QStringLiteral("bannerinfoempty") << BannerInfo();
}
