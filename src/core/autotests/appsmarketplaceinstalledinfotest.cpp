/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "appsmarketplaceinstalledinfotest.h"
#include "apps/appsmarketplaceinstalledinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AppsMarketPlaceInstalledInfoTest)

AppsMarketPlaceInstalledInfoTest::AppsMarketPlaceInstalledInfoTest(QObject *parent)
    : QObject{parent}
{
}

void AppsMarketPlaceInstalledInfoTest::shouldHaveDefaultValues()
{
    AppsMarketPlaceInstalledInfo w;
    QVERIFY(!w.isPrivate());
    QVERIFY(!w.migrated());
    QVERIFY(w.appName().isEmpty());
    QVERIFY(w.version().isEmpty());
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.support().isEmpty());
    QVERIFY(w.homePage().isEmpty());
    QVERIFY(w.authorName().isEmpty());
    QVERIFY(w.appId().isEmpty());
    QCOMPARE(w.status(), AppsMarketPlaceInstalledInfo::Status::Unknown);
}

void AppsMarketPlaceInstalledInfoTest::shouldLoadAppsMarketPlaceInstalledInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AppsMarketPlaceInstalledInfo>("appsmaketplaceinstalledinfo");

    QTest::addRow("empty") << u"empty"_s << AppsMarketPlaceInstalledInfo();

    {
        AppsMarketPlaceInstalledInfo appsmaketplaceinstalledinfo;
        appsmaketplaceinstalledinfo.setAppName("Jitsi"_L1);
        appsmaketplaceinstalledinfo.setIsPrivate(false);
        appsmaketplaceinstalledinfo.setVersion("2.0.4"_L1);
        appsmaketplaceinstalledinfo.setDescription("Jitsi Conference Calls Provider"_L1);
        appsmaketplaceinstalledinfo.setSupport("https://github.com/RocketChat/Rocket.Chat"_L1);
        appsmaketplaceinstalledinfo.setHomePage("https://github.com/RocketChat/Rocket.Chat"_L1);
        appsmaketplaceinstalledinfo.setAuthorName("Rocket.Chat Technologies Corp."_L1);
        appsmaketplaceinstalledinfo.setMigrated(true);
        appsmaketplaceinstalledinfo.setAppId("3b387ba9-f57c-44c6-9810-8c0256abd64c");
        appsmaketplaceinstalledinfo.setStatus(AppsMarketPlaceInstalledInfo::Status::AutoEnabled);
        QTest::addRow("appsinstalled1") << u"appsinstalled1"_s << appsmaketplaceinstalledinfo;
    }
}

void AppsMarketPlaceInstalledInfoTest::shouldLoadAppsMarketPlaceInstalledInfo()
{
    QFETCH(QString, name);
    QFETCH(AppsMarketPlaceInstalledInfo, appsmaketplaceinstalledinfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/appsmarketplaceinstalledinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    AppsMarketPlaceInstalledInfo m;
    m.parseInstalledAppsMarketPlaceInfo(obj);
    QCOMPARE(m, appsmaketplaceinstalledinfo);
}

#include "moc_appsmarketplaceinstalledinfotest.cpp"
