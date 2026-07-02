/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "licensesmanagertest.h"
#include "licenses/licensesmanager.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(LicensesManagerTest)
LicensesManagerTest::LicensesManagerTest(QObject *parent)
    : QObject{parent}
{
}

void LicensesManagerTest::shouldHaveDefaultValues()
{
    LicensesManager w;
    QVERIFY(w.activeModules().isEmpty());
    QVERIFY(!w.hasLicense(LicensesManager::ActiveModule::Unknown));
    QVERIFY(!w.hasLicense(LicensesManager::ActiveModule::LdapEnterprise));
}

void LicensesManagerTest::shouldParseLicenses_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QList<LicensesManager::ActiveModule>>("licenses");
    {
        const QList<LicensesManager::ActiveModule> licenses{LicensesManager::convertStringToActiveModule(u"auditing"_s),
                                                            LicensesManager::convertStringToActiveModule(u"canned-responses"_s),
                                                            LicensesManager::convertStringToActiveModule(u"ldap-enterprise"_s),
                                                            LicensesManager::convertStringToActiveModule(u"livechat-enterprise"_s),
                                                            LicensesManager::convertStringToActiveModule(u"voip-enterprise"_s),
                                                            LicensesManager::convertStringToActiveModule(u"omnichannel-mobile-enterprise"_s),
                                                            LicensesManager::convertStringToActiveModule(u"engagement-dashboard"_s),
                                                            LicensesManager::convertStringToActiveModule(u"push-privacy"_s),
                                                            LicensesManager::convertStringToActiveModule(u"scalability"_s),
                                                            LicensesManager::convertStringToActiveModule(u"teams-mention"_s),
                                                            LicensesManager::convertStringToActiveModule(u"saml-enterprise"_s),
                                                            LicensesManager::convertStringToActiveModule(u"oauth-enterprise"_s),
                                                            LicensesManager::convertStringToActiveModule(u"device-management"_s),
                                                            LicensesManager::convertStringToActiveModule(u"federation"_s),
                                                            LicensesManager::convertStringToActiveModule(u"videoconference-enterprise"_s),
                                                            LicensesManager::convertStringToActiveModule(u"message-read-receipt"_s),
                                                            LicensesManager::convertStringToActiveModule(u"outlook-calendar"_s),
                                                            LicensesManager::convertStringToActiveModule(u"hide-watermark"_s),
                                                            LicensesManager::convertStringToActiveModule(u"custom-roles"_s),
                                                            LicensesManager::convertStringToActiveModule(u"accessibility-certification"_s)};
        QTest::newRow("test1.json") << u"test1"_s << licenses;
    }
}

void LicensesManagerTest::shouldParseLicenses()
{
    QFETCH(QString, fileName);
    QFETCH(QList<LicensesManager::ActiveModule>, licenses);

    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/licensesmanager/"_L1 + fileName + ".json"_L1;
    const QJsonObject fields = AutoTestHelper::loadJsonObject(originalJsonFile);

    LicensesManager m;
    m.parseLicenses(fields);
    const bool equal = (licenses == m.activeModules());
    if (!equal) {
        qDebug() << " current value " << m.activeModules();
        qDebug() << " expected value " << licenses;
    }
    QVERIFY(equal);
}

void LicensesManagerTest::shouldConvertFromString()
{
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"auditing"_s), LicensesManager::ActiveModule::Auditing);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"canned-responses"_s), LicensesManager::ActiveModule::CannedResponses);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"ldap-enterprise"_s), LicensesManager::ActiveModule::LdapEnterprise);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"livechat-enterprise"_s), LicensesManager::ActiveModule::LivechatEnterprise);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"voip-enterprise"_s), LicensesManager::ActiveModule::VoipEnterprise);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"omnichannel-mobile-enterprise"_s), LicensesManager::ActiveModule::OmnichannelMobileEnterprise);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"engagement-dashboard"_s), LicensesManager::ActiveModule::EngagementDashboard);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"push-privacy"_s), LicensesManager::ActiveModule::PushPrivacy);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"scalability"_s), LicensesManager::ActiveModule::Scalability);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"teams-mention"_s), LicensesManager::ActiveModule::TeamsMention);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"saml-enterprise"_s), LicensesManager::ActiveModule::SamlEnterprise);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"oauth-enterprise"_s), LicensesManager::ActiveModule::OauthEnterprise);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"device-management"_s), LicensesManager::ActiveModule::DeviceManagement);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"federation"_s), LicensesManager::ActiveModule::Federation);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"videoconference-enterprise"_s), LicensesManager::ActiveModule::VideoconferenceEnterprise);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"message-read-receipt"_s), LicensesManager::ActiveModule::MessageReadReceipt);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"outlook-calendar"_s), LicensesManager::ActiveModule::OutlookCalendar);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"hide-watermark"_s), LicensesManager::ActiveModule::HideWatermark);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"custom-roles"_s), LicensesManager::ActiveModule::CustomRoles);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"accessibility-certification"_s), LicensesManager::ActiveModule::AccessibilityCertification);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"abac"_s), LicensesManager::ActiveModule::Abac);
    QCOMPARE(LicensesManager::convertStringToActiveModule(u"foo"_s), LicensesManager::ActiveModule::Unknown);
    QCOMPARE(LicensesManager::convertStringToActiveModule(QString()), LicensesManager::ActiveModule::Unknown);
}

void LicensesManagerTest::shouldConvertToString()
{
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::Auditing), u"auditing"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::CannedResponses), u"canned-responses"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::LdapEnterprise), u"ldap-enterprise"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::LivechatEnterprise), u"livechat-enterprise"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::VoipEnterprise), u"voip-enterprise"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::OmnichannelMobileEnterprise), u"omnichannel-mobile-enterprise"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::EngagementDashboard), u"engagement-dashboard"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::PushPrivacy), u"push-privacy"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::Scalability), u"scalability"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::TeamsMention), u"teams-mention"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::SamlEnterprise), u"saml-enterprise"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::OauthEnterprise), u"oauth-enterprise"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::DeviceManagement), u"device-management"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::Federation), u"federation"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::VideoconferenceEnterprise), u"videoconference-enterprise"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::MessageReadReceipt), u"message-read-receipt"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::OutlookCalendar), u"outlook-calendar"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::HideWatermark), u"hide-watermark"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::CustomRoles), u"custom-roles"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::AccessibilityCertification), u"accessibility-certification"_s);
    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::Abac), u"abac"_s);

    QCOMPARE(LicensesManager::convertActiveModuleToString(LicensesManager::ActiveModule::Unknown), QString());
}

#include "moc_licensesmanagertest.cpp"
