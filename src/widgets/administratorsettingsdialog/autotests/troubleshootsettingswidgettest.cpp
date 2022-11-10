/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidgettest.h"
#include "administratorsettingsdialog/troubleshoot/troubleshootsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>
#include <QTest>

QTEST_MAIN(TroubleshootSettingsWidgetTest)
TroubleshootSettingsWidgetTest::TroubleshootSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TroubleshootSettingsWidgetTest::shouldHaveDefaultValues()
{
    TroubleshootSettingsWidget w(nullptr);

    auto mDisableNotifications = w.findChild<QCheckBox *>(QStringLiteral("mDisableNotifications"));
    QVERIFY(mDisableNotifications);
    QVERIFY(!mDisableNotifications->isChecked());
    QVERIFY(!mDisableNotifications->text().isEmpty());
    QVERIFY(mDisableNotifications->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableNotifications), QStringLiteral("Troubleshoot_Disable_Notifications"));

    auto mDisablePresenceBroadcast = w.findChild<QCheckBox *>(QStringLiteral("mDisablePresenceBroadcast"));
    QVERIFY(mDisablePresenceBroadcast);
    QVERIFY(!mDisablePresenceBroadcast->isChecked());
    QVERIFY(!mDisablePresenceBroadcast->text().isEmpty());
    QVERIFY(mDisablePresenceBroadcast->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisablePresenceBroadcast), QStringLiteral("Troubleshoot_Disable_Presence_Broadcast"));

    auto mDisableInstanceBroadcast = w.findChild<QCheckBox *>(QStringLiteral("mDisableInstanceBroadcast"));
    QVERIFY(mDisableInstanceBroadcast);
    QVERIFY(!mDisableInstanceBroadcast->isChecked());
    QVERIFY(!mDisableInstanceBroadcast->text().isEmpty());
    QVERIFY(mDisableInstanceBroadcast->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableInstanceBroadcast), QStringLiteral("Troubleshoot_Disable_Instance_Broadcast"));

    auto mDisableSessionsMonitor = w.findChild<QCheckBox *>(QStringLiteral("mDisableSessionsMonitor"));
    QVERIFY(mDisableSessionsMonitor);
    QVERIFY(!mDisableSessionsMonitor->isChecked());
    QVERIFY(!mDisableSessionsMonitor->text().isEmpty());
    QVERIFY(mDisableSessionsMonitor->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableSessionsMonitor), QStringLiteral("Troubleshoot_Disable_Sessions_Monitor"));
}
