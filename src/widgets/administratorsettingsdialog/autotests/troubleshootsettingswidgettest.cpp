/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mDisableNotifications = w.findChild<QCheckBox *>(u"mDisableNotifications"_s);
    QVERIFY(mDisableNotifications);
    QVERIFY(!mDisableNotifications->isChecked());
    QVERIFY(!mDisableNotifications->text().isEmpty());
    QVERIFY(!mDisableNotifications->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableNotifications), u"Troubleshoot_Disable_Notifications"_s);

    auto mDisablePresenceBroadcast = w.findChild<QCheckBox *>(u"mDisablePresenceBroadcast"_s);
    QVERIFY(mDisablePresenceBroadcast);
    QVERIFY(!mDisablePresenceBroadcast->isChecked());
    QVERIFY(!mDisablePresenceBroadcast->text().isEmpty());
    QVERIFY(!mDisablePresenceBroadcast->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisablePresenceBroadcast), u"Troubleshoot_Disable_Presence_Broadcast"_s);

    auto mDisableInstanceBroadcast = w.findChild<QCheckBox *>(u"mDisableInstanceBroadcast"_s);
    QVERIFY(mDisableInstanceBroadcast);
    QVERIFY(!mDisableInstanceBroadcast->isChecked());
    QVERIFY(!mDisableInstanceBroadcast->text().isEmpty());
    QVERIFY(!mDisableInstanceBroadcast->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableInstanceBroadcast), u"Troubleshoot_Disable_Instance_Broadcast"_s);

    auto mDisableSessionsMonitor = w.findChild<QCheckBox *>(u"mDisableSessionsMonitor"_s);
    QVERIFY(mDisableSessionsMonitor);
    QVERIFY(!mDisableSessionsMonitor->isChecked());
    QVERIFY(!mDisableSessionsMonitor->text().isEmpty());
    QVERIFY(!mDisableSessionsMonitor->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableSessionsMonitor), u"Troubleshoot_Disable_Sessions_Monitor"_s);

    auto mDisableLivechatActivityMonitor = w.findChild<QCheckBox *>(u"mDisableLivechatActivityMonitor"_s);
    QVERIFY(mDisableLivechatActivityMonitor);
    QVERIFY(!mDisableLivechatActivityMonitor->isChecked());
    QVERIFY(!mDisableLivechatActivityMonitor->text().isEmpty());
    QVERIFY(!mDisableLivechatActivityMonitor->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableLivechatActivityMonitor), u"Troubleshoot_Disable_Livechat_Activity_Monitor"_s);

    auto mDisableStatisticsGenerator = w.findChild<QCheckBox *>(u"mDisableStatisticsGenerator"_s);
    QVERIFY(mDisableStatisticsGenerator);
    QVERIFY(!mDisableStatisticsGenerator->isChecked());
    QVERIFY(!mDisableStatisticsGenerator->text().isEmpty());
    QVERIFY(!mDisableStatisticsGenerator->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableStatisticsGenerator), u"Troubleshoot_Disable_Statistics_Generator"_s);

    auto mDisableDataExporterProcessor = w.findChild<QCheckBox *>(u"mDisableDataExporterProcessor"_s);
    QVERIFY(mDisableDataExporterProcessor);
    QVERIFY(!mDisableDataExporterProcessor->isChecked());
    QVERIFY(!mDisableDataExporterProcessor->text().isEmpty());
    QVERIFY(!mDisableDataExporterProcessor->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableDataExporterProcessor), u"Troubleshoot_Disable_Data_Exporter_Processor"_s);

    auto mDisableWorkspaceSync = w.findChild<QCheckBox *>(u"mDisableWorkspaceSync"_s);
    QVERIFY(mDisableWorkspaceSync);
    QVERIFY(!mDisableWorkspaceSync->isChecked());
    QVERIFY(!mDisableWorkspaceSync->text().isEmpty());
    QVERIFY(!mDisableWorkspaceSync->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableWorkspaceSync), u"Troubleshoot_Disable_Workspace_Sync"_s);

    auto mDisableTeamsMention = w.findChild<QCheckBox *>(u"mDisableTeamsMention"_s);
    QVERIFY(mDisableTeamsMention);
    QVERIFY(!mDisableTeamsMention->isChecked());
    QVERIFY(!mDisableTeamsMention->text().isEmpty());
    QVERIFY(!mDisableTeamsMention->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisableTeamsMention), u"Troubleshoot_Disable_Teams_Mention"_s);
}

#include "moc_troubleshootsettingswidgettest.cpp"
