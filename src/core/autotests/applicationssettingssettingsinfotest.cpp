/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingsinfotest.h"
#include "apps/applicationssettingssettingsinfo.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ApplicationsSettingsSettingsInfoTest)
ApplicationsSettingsSettingsInfoTest::ApplicationsSettingsSettingsInfoTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsSettingsInfoTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsSettingsInfo w;
    QVERIFY(w.i18nDescription().isEmpty());
    QVERIFY(w.i18nLabel().isEmpty());
    QVERIFY(w.id().isEmpty());
    QVERIFY(!w.packageValue().isValid());
    QVERIFY(!w.value().isValid());
    QVERIFY(w.values().isEmpty());
    QVERIFY(!w.required());
    QVERIFY(!w.multiLine());
    QCOMPARE(w.settingType(), ApplicationsSettingsSettingsInfo::SettingType::Unknown);
}

void ApplicationsSettingsSettingsInfoTest::shouldLoadApplicationSettingsInfo_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ApplicationsSettingsSettingsInfo>("info");

    {
        ApplicationsSettingsSettingsInfo info;
        info.setI18nDescription("jitsi_application_id_description"_L1);
        info.setI18nLabel("jitsi_application_id"_L1);
        info.setId("jitsi_application_id"_L1);
        info.setSettingType(ApplicationsSettingsSettingsInfo::SettingType::String);
        info.setPackageValue(u""_s);
        QTest::addRow("test1") << u"test1"_s << info;
    }

    {
        ApplicationsSettingsSettingsInfo info;
        info.setI18nDescription("WordReplacerApp_Filters_Description"_L1);
        info.setI18nLabel("WordReplacerApp_Filters"_L1);
        info.setId("filters"_L1);
        info.setSettingType(ApplicationsSettingsSettingsInfo::SettingType::String);
        info.setRequired(true);
        info.setMultiLine(true);
        info.setPackageValue(
            QStringLiteral("[{\"search\": \"#(\\\\d+)\", \"replace\": \"[$&](https://www1.example.com/issues/$1)\"}, {\"search\": \"BUG-(\\\\d+)\", "
                           "\"replace\": \"[$&](https://www2.example.com/issues/$1)\"}]"));
        QTest::addRow("test2") << u"test2"_s << info;
    }
}

void ApplicationsSettingsSettingsInfoTest::shouldLoadApplicationSettingsInfo()
{
    QFETCH(QString, name);
    QFETCH(ApplicationsSettingsSettingsInfo, info);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/applicationssettingssettingsinfo/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    ApplicationsSettingsSettingsInfo m;
    m.parseSettings(obj);
    QCOMPARE(m, info);
}

#include "moc_applicationssettingssettingsinfotest.cpp"
