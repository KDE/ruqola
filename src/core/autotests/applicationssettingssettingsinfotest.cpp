/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingsinfotest.h"
#include "apps/applicationssettingssettingsinfo.h"
#include "ruqola_autotest_helper.h"
#include <QJsonArray>
#include <QJsonObject>
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

void ApplicationsSettingsSettingsInfoTest::shouldResetStateWhenParsingTwice()
{
    ApplicationsSettingsSettingsInfo info;

    const QJsonObject firstObj{
        {"i18nDescription"_L1, "desc1"_L1},
        {"i18nLabel"_L1, "label1"_L1},
        {"id"_L1, "id1"_L1},
        {"type"_L1, "select"_L1},
        {"packageValue"_L1, "one"_L1},
        {"value"_L1, "one"_L1},
        {"values"_L1,
         QJsonArray{
             QJsonObject{{"key"_L1, "one"_L1}, {"i18nLabel"_L1, "One"_L1}},
             QJsonObject{{"key"_L1, "two"_L1}, {"i18nLabel"_L1, "Two"_L1}},
         }},
    };
    info.parseSettings(firstObj);
    QVERIFY(!info.values().isEmpty());
    QCOMPARE(info.value().toString(), "one"_L1);

    const QJsonObject secondObj{
        {"i18nDescription"_L1, "desc2"_L1},
        {"i18nLabel"_L1, "label2"_L1},
        {"id"_L1, "id2"_L1},
        {"type"_L1, "string"_L1},
        {"packageValue"_L1, "default"_L1},
    };
    info.parseSettings(secondObj);

    QVERIFY(info.values().isEmpty());
    QVERIFY(!info.value().isValid());
    QCOMPARE(info.packageValue().toString(), "default"_L1);
    QCOMPARE(info.settingType(), ApplicationsSettingsSettingsInfo::SettingType::String);
}

#include "moc_applicationssettingssettingsinfotest.cpp"
