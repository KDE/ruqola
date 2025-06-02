/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

TroubleshootSettingsWidget::TroubleshootSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mDisableNotifications(new QCheckBox(i18nc("@option:check", "Disable Notifications"), this))
    , mDisablePresenceBroadcast(new QCheckBox(i18nc("@option:check", "Disable Presence Broadcast"), this))
    , mDisableInstanceBroadcast(new QCheckBox(i18nc("@option:check", "Disable Instance Broadcast"), this))
    , mDisableSessionsMonitor(new QCheckBox(i18nc("@option:check", "Disable Sessions Monitor"), this))
    , mDisableLivechatActivityMonitor(new QCheckBox(i18nc("@option:check", "Disable Livechat Activity Monitor"), this))
    , mDisableStatisticsGenerator(new QCheckBox(i18nc("@option:check", "Disable Statistics Generator"), this))
    , mDisableDataExporterProcessor(new QCheckBox(i18nc("@option:check", "Disable Data Exporter Processor"), this))
    , mDisableWorkspaceSync(new QCheckBox(i18nc("@option:check", "Disable Workspace Sync"), this))
    , mDisableTeamsMention(new QCheckBox(i18nc("@option:check", "Disable Teams Mention"), this))
{
    mDisableNotifications->setObjectName(QStringLiteral("mDisableNotifications"));
    mDisableNotifications->setToolTip(
        i18n("This setting completely disables the notifications system; sounds, desktop notifications, mobile notifications, and emails will stop!"));
    addCheckBox(mDisableNotifications, QStringLiteral("Troubleshoot_Disable_Notifications"));

    mDisablePresenceBroadcast->setObjectName(QStringLiteral("mDisablePresenceBroadcast"));
    mDisablePresenceBroadcast->setToolTip(
        i18n("This setting prevents all instances from sending the status changes of the users to their clients keeping all the users with their presence "
             "status from the first load!"));
    addCheckBox(mDisablePresenceBroadcast, QStringLiteral("Troubleshoot_Disable_Presence_Broadcast"));

    mDisableInstanceBroadcast->setObjectName(QStringLiteral("mDisableInstanceBroadcast"));
    mDisableInstanceBroadcast->setToolTip(
        i18n("This setting prevents the Rocket.Chat instances from sending events to the other instances, it may cause syncing problems and misbehavior!"));
    addCheckBox(mDisableInstanceBroadcast, QStringLiteral("Troubleshoot_Disable_Instance_Broadcast"));

    mDisableSessionsMonitor->setObjectName(QStringLiteral("mDisableSessionsMonitor"));
    mDisableSessionsMonitor->setToolTip(
        i18n("This setting prevents the Rocket.Chat instances from sending events to the other instances, it may cause syncing problems and misbehavior!"));
    addCheckBox(mDisableSessionsMonitor, QStringLiteral("Troubleshoot_Disable_Sessions_Monitor"));

    mDisableLivechatActivityMonitor->setObjectName(QStringLiteral("mDisableLivechatActivityMonitor"));
    mDisableLivechatActivityMonitor->setToolTip(
        i18n("This setting stops the processing of livechat visitor sessions causing the statistics to stop working correctly!"));
    addCheckBox(mDisableLivechatActivityMonitor, QStringLiteral("Troubleshoot_Disable_Livechat_Activity_Monitor"));

    mDisableStatisticsGenerator->setObjectName(QStringLiteral("mDisableStatisticsGenerator"));
    mDisableStatisticsGenerator->setToolTip(
        i18n("This setting stops the processing all statistics making the info page outdated until someone clicks on the refresh button and may cause other "
             "missing information around the system!"));
    addCheckBox(mDisableStatisticsGenerator, QStringLiteral("Troubleshoot_Disable_Statistics_Generator"));

    mDisableDataExporterProcessor->setObjectName(QStringLiteral("mDisableDataExporterProcessor"));
    mDisableDataExporterProcessor->setToolTip(
        i18n("This setting stops the processing of all export requests from users, so they will not receive the link to download their data!"));
    addCheckBox(mDisableDataExporterProcessor, QStringLiteral("Troubleshoot_Disable_Data_Exporter_Processor"));

    mDisableWorkspaceSync->setObjectName(QStringLiteral("mDisableWorkspaceSync"));
    mDisableWorkspaceSync->setToolTip(
        i18n("This setting stops the sync of this server with Rocket.Chat's cloud and may cause issues with marketplace and enterprise licenses!"));
    addCheckBox(mDisableWorkspaceSync, QStringLiteral("Troubleshoot_Disable_Workspace_Sync"));

    mDisableTeamsMention->setObjectName(QStringLiteral("mDisableTeamsMention"));
    mDisableTeamsMention->setToolTip(
        i18n("This setting disables the teams mention feature. User's won't be able to mention a Team by name in a message and get its members notified."));
    addCheckBox(mDisableTeamsMention, QStringLiteral("Troubleshoot_Disable_Teams_Mention"));
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
    initializeWidget(mDisableTeamsMention, mapSettings, false);
}

#include "moc_troubleshootsettingswidget.cpp"
