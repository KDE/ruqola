/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "appsmarketplaceinfotest.h"
#include "apps/appsmarketplaceinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
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
    QVERIFY(d.homePage().isEmpty());
    QVERIFY(d.authorName().isEmpty());
    QVERIFY(d.support().isEmpty());
    QCOMPARE(d.requested(), 0);
    QVERIFY(!d.installedInfo().isValid());

    // 03/07/2024: size: 256
    QCOMPARE(sizeof(AppsMarketPlaceInfo), 584);
}

void AppsMarketPlaceInfoTest::shouldLoadAppsMarketPlaceInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<AppsMarketPlaceInfo>("appsmaketplaceinfo");

    QTest::addRow("empty") << u"empty"_s << AppsMarketPlaceInfo();

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
        appsMarketPlaceInfo.setAuthorName("Zakhar Petrov"_L1);
        appsMarketPlaceInfo.setModifiedDate(1633074321015);
        appsMarketPlaceInfo.setShortDescription({});
        appsMarketPlaceInfo.setHomePage("https://github.com/zakhar-petrov"_L1);
        appsMarketPlaceInfo.setSupport("https://github.com/zakhar-petrov/rocketchat-youtrack-linker/issues"_L1);

        QTest::addRow("apps1") << u"apps1"_s << appsMarketPlaceInfo;
    }
    {
        AppsMarketPlaceInfo appsMarketPlaceInfo;
        appsMarketPlaceInfo.setAppId(QByteArray("3b387ba9-f57c-44c6-9810-8c0256abd64c"));
        appsMarketPlaceInfo.setIsEnterpriseOnly(false);
        appsMarketPlaceInfo.setAppName("Jitsi"_L1);
        appsMarketPlaceInfo.setCategories({"Voice & Video"_L1, "Communication"_L1, "Productivity"_L1});
        appsMarketPlaceInfo.setDocumentationUrl("docs.rocket.chat/use-rocket.chat/rocket.chat-conference-call/conference-call-admin-guide/jitsi-app"_L1);
        appsMarketPlaceInfo.setPurchaseType("buy"_L1);
        appsMarketPlaceInfo.setDescription("<p>Jitsi is a collection of open-source applications</p>\n"_L1);
        appsMarketPlaceInfo.setPrice(0);
        appsMarketPlaceInfo.setAuthorName("Rocket.Chat Technologies Corp."_L1);
        appsMarketPlaceInfo.setVersion("2.1.1"_L1);
        appsMarketPlaceInfo.setModifiedDate(1706210343886);
        appsMarketPlaceInfo.setShortDescription("Enjoy a secure and robust video conferencing"_L1);
        appsMarketPlaceInfo.setHomePage("https://github.com/RocketChat/Rocket.Chat"_L1);
        appsMarketPlaceInfo.setSupport("support@rocket.chat"_L1);
        appsMarketPlaceInfo.setPrivacyPolicySummary(
            "We use your data to provide and improve the Service. By using the Service, you agree to the collection and use of information in accordance with this policy. Unless otherwise defined in this Privacy Policy, the terms used in this Privacy Policy have the same meanings as in our Terms and Conditions.\n"_L1);

        QTest::addRow("apps2") << u"apps2"_s << appsMarketPlaceInfo;
    }

    {
        AppsMarketPlaceInfo appsMarketPlaceInfo;
        appsMarketPlaceInfo.setAppId(QByteArray("6acc91d8-f7db-4c1f-91df-7fd8f84d716c"));
        appsMarketPlaceInfo.setIsEnterpriseOnly(false);
        appsMarketPlaceInfo.setAppName("InStatus Notifier"_L1);
        appsMarketPlaceInfo.setCategories({"Bots"_L1, "Developer Tools"_L1});
        appsMarketPlaceInfo.setPurchaseType("buy"_L1);
        appsMarketPlaceInfo.setDescription(
            "<p>Get alerts from InStatus. Run /instatus command on any channel where youd like to get alerts and then follow the instructions.</p>\n"_L1);
        appsMarketPlaceInfo.setPrice(0);
        appsMarketPlaceInfo.setVersion("1.0.0"_L1);
        appsMarketPlaceInfo.setModifiedDate(1655134588065);
        appsMarketPlaceInfo.setHomePage("https://github.com/murtaza98/Instatus-RocketChat-App"_L1);
        appsMarketPlaceInfo.setSupport("https://github.com/murtaza98/Instatus-RocketChat-App/issues"_L1);
        appsMarketPlaceInfo.setPrivacyPolicySummary("The app doesn't transfer any data to an external server outside your Rocket.Chat instance."_L1);
        appsMarketPlaceInfo.setRequested(1);
        appsMarketPlaceInfo.setAuthorName("Murtaza Patrawala"_L1);

        QTest::addRow("apps3-requested") << u"apps3-requested"_s << appsMarketPlaceInfo;
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
