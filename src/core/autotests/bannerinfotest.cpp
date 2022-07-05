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
    {
        BannerInfo info;
        info.setIdentifier(QStringLiteral("alert-5fcc1f02f5204d09050943d2"));
        info.setLink(QStringLiteral("https://github.com/RocketChat/Rocket.Chat/releases/tag/3.9.1"));
        info.setPriority(10);
        info.setRead(true);
        info.setText(QStringLiteral("For all installations using SAML Please upgrade as soon as possible.  3.9.1 / 3.8.3 / 3.7.3 / 2.4.13 / 1.3.4 / 0.74.4"));
        info.setTitle(QStringLiteral("Attn: Important Security fix"));
        QTest::addRow("bannerinfo1") << QStringLiteral("bannerinfo1") << info;
    }
    {
        BannerInfo info;
        info.setIdentifier(QStringLiteral("versionUpdate-4_4_1"));
        info.setLink(QStringLiteral("https://github.com/RocketChat/Rocket.Chat/releases/tag/4.4.1"));
        info.setPriority(10);
        info.setText(QStringLiteral("New_version_available_(s)"));
        info.setTextArguments(QStringList({QStringLiteral("4.4.1")}));
        info.setTitle(QStringLiteral("Update_your_RocketChat"));
        QTest::addRow("bannerinfo2") << QStringLiteral("bannerinfo2") << info;
    }
}
