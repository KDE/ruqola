/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

TroubleshootSettingsWidget::TroubleshootSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mDisableNotifications(new QCheckBox(i18n("Disable Notifications"), this))
    , mDisablePresenceBroadcast(new QCheckBox(i18n("Disable Presence Broadcast"), this))
    , mDisableInstanceBroadcast(new QCheckBox(i18n("Disable Instance Broadcast"), this))
    , mDisableSessionsMonitor(new QCheckBox(i18n("Disable Sessions Monitor"), this))
    , mDisableLivechatActivityMonitor(new QCheckBox(i18n("Disable Livechat Activity Monitor"), this))
    , mDisableStatisticsGenerator(new QCheckBox(i18n("Disable Statistics Generator"), this))
    , mDisableDataExporterProcessor(new QCheckBox(i18n("Disable Data Exporter Processor"), this))
    , mDisableWorkspaceSync(new QCheckBox(i18n("Disable Workspace Sync"), this))
{
    mDisableNotifications->setObjectName(QStringLiteral("mDisableNotifications"));
    mMainLayout->addWidget(mDisableNotifications);
    mDisableNotifications->setToolTip(
        i18n("This setting completely disables the notifications system; sounds, desktop notifications, mobile notifications, and emails will stop!"));
    connectCheckBox(mDisableNotifications, QStringLiteral("Troubleshoot_Disable_Notifications"));

    mDisablePresenceBroadcast->setObjectName(QStringLiteral("mDisablePresenceBroadcast"));
    mMainLayout->addWidget(mDisablePresenceBroadcast);
    mDisablePresenceBroadcast->setToolTip(
        i18n("This setting prevents all instances from sending the status changes of the users to their clients keeping all the users with their presence "
             "status from the first load!"));
    connectCheckBox(mDisablePresenceBroadcast, QStringLiteral("Troubleshoot_Disable_Presence_Broadcast"));

    mDisableInstanceBroadcast->setObjectName(QStringLiteral("mDisableInstanceBroadcast"));
    mMainLayout->addWidget(mDisableInstanceBroadcast);
    mDisableInstanceBroadcast->setToolTip(
        i18n("This setting prevents the Rocket.Chat instances from sending events to the other instances, it may cause syncing problems and misbehavior!"));
    connectCheckBox(mDisableInstanceBroadcast, QStringLiteral("Troubleshoot_Disable_Instance_Broadcast"));

    mDisableSessionsMonitor->setObjectName(QStringLiteral("mDisableSessionsMonitor"));
    mMainLayout->addWidget(mDisableSessionsMonitor);
    mDisableSessionsMonitor->setToolTip(
        i18n("This setting prevents the Rocket.Chat instances from sending events to the other instances, it may cause syncing problems and misbehavior!"));
    connectCheckBox(mDisableSessionsMonitor, QStringLiteral("Troubleshoot_Disable_Sessions_Monitor"));

    mDisableLivechatActivityMonitor->setObjectName(QStringLiteral("mDisableLivechatActivityMonitor"));
    mMainLayout->addWidget(mDisableLivechatActivityMonitor);
    mDisableLivechatActivityMonitor->setToolTip(
        i18n("This setting stops the processing of livechat visitor sessions causing the statistics to stop working correctly!"));
    connectCheckBox(mDisableLivechatActivityMonitor, QStringLiteral("Troubleshoot_Disable_Livechat_Activity_Monitor"));

    mDisableStatisticsGenerator->setObjectName(QStringLiteral("mDisableStatisticsGenerator"));
    mMainLayout->addWidget(mDisableStatisticsGenerator);
    mDisableStatisticsGenerator->setToolTip(
        i18n("This setting stops the processing all statistics making the info page outdated until someone clicks on the refresh button and may cause other "
             "missing information around the system!"));
    connectCheckBox(mDisableStatisticsGenerator, QStringLiteral("Troubleshoot_Disable_Statistics_Generator"));

    mDisableDataExporterProcessor->setObjectName(QStringLiteral("mDisableDataExporterProcessor"));
    mMainLayout->addWidget(mDisableDataExporterProcessor);
    mDisableDataExporterProcessor->setToolTip(
        i18n("This setting stops the processing of all export requests from users, so they will not receive the link to download their data!"));
    connectCheckBox(mDisableDataExporterProcessor, QStringLiteral("Troubleshoot_Disable_Data_Exporter_Processor"));

    mDisableWorkspaceSync->setObjectName(QStringLiteral("mDisableWorkspaceSync"));
    mMainLayout->addWidget(mDisableWorkspaceSync);
    mDisableWorkspaceSync->setToolTip(
        i18n("This setting stops the sync of this server with Rocket.Chat's cloud and may cause issues with marketplace and enterprise licenses!"));
    connectCheckBox(mDisableWorkspaceSync, QStringLiteral("Troubleshoot_Disable_Workspace_Sync"));
}

TroubleshootSettingsWidget::~TroubleshootSettingsWidget() = default;

void TroubleshootSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mDisableNotifications, mapSettings, false);
    initializeWidget(mDisablePresenceBroadcast, mapSettings, false);
    initializeWidget(mDisableInstanceBroadcast, mapSettings, false);
    initializeWidget(mDisableSessionsMonitor, mapSettings, false);
    initializeWidget(mDisableLivechatActivityMonitor, mapSettings, false);
    initializeWidget(mDisableStatisticsGenerator, mapSettings, false);
    initializeWidget(mDisableDataExporterProcessor, mapSettings, false);
    initializeWidget(mDisableWorkspaceSync, mapSettings, false);
}
