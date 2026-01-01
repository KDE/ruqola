/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "bannerinfotest.h"
#include "bannerinfo/bannerinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
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
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/bannerinfo/"_L1 + name + ".json"_L1;
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
    QTest::addRow("bannerinfoempty") << u"bannerinfoempty"_s << BannerInfo();
    {
        BannerInfo info;
        info.setIdentifier("alert-5fcc1f02f5204d09050943d2"_ba);
        info.setLink(u"https://github.com/RocketChat/Rocket.Chat/releases/tag/3.9.1"_s);
        info.setPriority(10);
        info.setRead(true);
        info.setText(u"For all installations using SAML Please upgrade as soon as possible.  3.9.1 / 3.8.3 / 3.7.3 / 2.4.13 / 1.3.4 / 0.74.4"_s);
        info.setTitle(u"Attn: Important Security fix"_s);
        QTest::addRow("bannerinfo1") << u"bannerinfo1"_s << info;
    }
    {
        BannerInfo info;
        info.setIdentifier("versionUpdate-4_4_1"_ba);
        info.setLink(u"https://github.com/RocketChat/Rocket.Chat/releases/tag/4.4.1"_s);
        info.setPriority(10);
        info.setText(u"New_version_available_(s)"_s);
        info.setTextArguments(QStringList({u"4.4.1"_s}));
        info.setTitle(u"Update_your_RocketChat"_s);
        info.setRead(false);
        QTest::addRow("bannerinfo2") << u"bannerinfo2"_s << info;
    }
}

#include "moc_bannerinfotest.cpp"
