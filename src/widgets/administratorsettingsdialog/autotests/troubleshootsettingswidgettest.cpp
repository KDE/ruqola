/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidgettest.h"
#include "administratorsettingsdialog/troubleshoot/troubleshootsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QFormLayout>

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
    QVERIFY(!mDisableNotifications->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableNotifications), QStringLiteral("Troubleshoot_Disable_Notifications"));

    auto mDisablePresenceBroadcast = w.findChild<QCheckBox *>(QStringLiteral("mDisablePresenceBroadcast"));
    QVERIFY(mDisablePresenceBroadcast);
    QVERIFY(!mDisablePresenceBroadcast->isChecked());
    QVERIFY(!mDisablePresenceBroadcast->text().isEmpty());
    QVERIFY(!mDisablePresenceBroadcast->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisablePresenceBroadcast), QStringLiteral("Troubleshoot_Disable_Presence_Broadcast"));

    auto mDisableInstanceBroadcast = w.findChild<QCheckBox *>(QStringLiteral("mDisableInstanceBroadcast"));
    QVERIFY(mDisableInstanceBroadcast);
    QVERIFY(!mDisableInstanceBroadcast->isChecked());
    QVERIFY(!mDisableInstanceBroadcast->text().isEmpty());
    QVERIFY(!mDisableInstanceBroadcast->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableInstanceBroadcast), QStringLiteral("Troubleshoot_Disable_Instance_Broadcast"));

    auto mDisableSessionsMonitor = w.findChild<QCheckBox *>(QStringLiteral("mDisableSessionsMonitor"));
    QVERIFY(mDisableSessionsMonitor);
    QVERIFY(!mDisableSessionsMonitor->isChecked());
    QVERIFY(!mDisableSessionsMonitor->text().isEmpty());
    QVERIFY(!mDisableSessionsMonitor->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableSessionsMonitor), QStringLiteral("Troubleshoot_Disable_Sessions_Monitor"));

    auto mDisableLivechatActivityMonitor = w.findChild<QCheckBox *>(QStringLiteral("mDisableLivechatActivityMonitor"));
    QVERIFY(mDisableLivechatActivityMonitor);
    QVERIFY(!mDisableLivechatActivityMonitor->isChecked());
    QVERIFY(!mDisableLivechatActivityMonitor->text().isEmpty());
    QVERIFY(!mDisableLivechatActivityMonitor->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableLivechatActivityMonitor), QStringLiteral("Troubleshoot_Disable_Livechat_Activity_Monitor"));

    auto mDisableStatisticsGenerator = w.findChild<QCheckBox *>(QStringLiteral("mDisableStatisticsGenerator"));
    QVERIFY(mDisableStatisticsGenerator);
    QVERIFY(!mDisableStatisticsGenerator->isChecked());
    QVERIFY(!mDisableStatisticsGenerator->text().isEmpty());
    QVERIFY(!mDisableStatisticsGenerator->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableStatisticsGenerator), QStringLiteral("Troubleshoot_Disable_Statistics_Generator"));

    auto mDisableDataExporterProcessor = w.findChild<QCheckBox *>(QStringLiteral("mDisableDataExporterProcessor"));
    QVERIFY(mDisableDataExporterProcessor);
    QVERIFY(!mDisableDataExporterProcessor->isChecked());
    QVERIFY(!mDisableDataExporterProcessor->text().isEmpty());
    QVERIFY(!mDisableDataExporterProcessor->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableDataExporterProcessor), QStringLiteral("Troubleshoot_Disable_Data_Exporter_Processor"));

    auto mDisableWorkspaceSync = w.findChild<QCheckBox *>(QStringLiteral("mDisableWorkspaceSync"));
    QVERIFY(mDisableWorkspaceSync);
    QVERIFY(!mDisableWorkspaceSync->isChecked());
    QVERIFY(!mDisableWorkspaceSync->text().isEmpty());
    QVERIFY(!mDisableWorkspaceSync->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableWorkspaceSync), QStringLiteral("Troubleshoot_Disable_Workspace_Sync"));
}

#include "moc_troubleshootsettingswidgettest.cpp"
