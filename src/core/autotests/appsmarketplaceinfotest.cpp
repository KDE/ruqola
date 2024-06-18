/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "appsmarketplaceinfotest.h"
#include "apps/appsmarketplaceinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AppsMarketPlaceInfoTest)
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

void AppsMarketPlaceInfoTest::shouldLoadAppsMarketPlaceInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AppsMarketPlaceInfo>("appsmaketplaceinfo");

    QTest::addRow("empty") << QStringLiteral("empty") << AppsMarketPlaceInfo();

    {
        AppsMarketPlaceInfo appsMarketPlaceInfo;
        appsMarketPlaceInfo.setAppId(QByteArray("9408b583-2f0e-4987-a341-daa171d761ce"));
        appsMarketPlaceInfo.setIsEnterpriseOnly(false);
        appsMarketPlaceInfo.setAppName("YouTrack Linker"_L1);
        appsMarketPlaceInfo.setCategories({"Developer Tools"_L1, "Productivity"_L1, "Project Management"_L1});
        appsMarketPlaceInfo.setDocumentationUrl({});
        appsMarketPlaceInfo.setPurchaseType("buy"_L1);
        appsMarketPlaceInfo.setDescription("Turns YouTrack references into links."_L1);
        appsMarketPlaceInfo.setPrice(0);
        appsMarketPlaceInfo.setVersion("1.2.1"_L1);
        appsMarketPlaceInfo.setModifiedDate(1633074321015);
        appsMarketPlaceInfo.setShortDescription({});

        QTest::addRow("apps1") << QStringLiteral("apps1") << appsMarketPlaceInfo;
    }
    {
        AppsMarketPlaceInfo appsMarketPlaceInfo;
        appsMarketPlaceInfo.setAppId(QByteArray("3b387ba9-f57c-44c6-9810-8c0256abd64c"));
        appsMarketPlaceInfo.setIsEnterpriseOnly(false);
        appsMarketPlaceInfo.setAppName("Jitsi"_L1);
        appsMarketPlaceInfo.setCategories({"Voice & Video"_L1, "Communication"_L1, "Productivity"_L1});
        appsMarketPlaceInfo.setDocumentationUrl("docs.rocket.chat/use-rocket.chat/rocket.chat-conference-call/conference-call-admin-guide/jitsi-app"_L1);
        appsMarketPlaceInfo.setPurchaseType("buy"_L1);
        appsMarketPlaceInfo.setDescription("Jitsi is a collection"_L1);
        appsMarketPlaceInfo.setPrice(0);
        appsMarketPlaceInfo.setVersion("2.1.1"_L1);
        appsMarketPlaceInfo.setModifiedDate(1706210343886);
        appsMarketPlaceInfo.setShortDescription("Enjoy a secure and robust video conferencing"_L1);

        QTest::addRow("apps2") << QStringLiteral("apps2") << appsMarketPlaceInfo;
    }
}

void AppsMarketPlaceInfoTest::shouldLoadAppsMarketPlaceInfo()
{
    QFETCH(QString, name);
    QFETCH(AppsMarketPlaceInfo, appsmaketplaceinfo);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/appsmarketplaceinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    AppsMarketPlaceInfo m;
    m.parseAppsMarketPlaceInfo(obj);
    QCOMPARE(m, appsmaketplaceinfo);
}

#include "moc_appsmarketplaceinfotest.cpp"
