/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "troubleshootsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mDisableNotifications->setObjectName(u"mDisableNotifications"_s);
    mDisableNotifications->setToolTip(
        i18n("This setting completely disables the notifications system; sounds, desktop notifications, mobile notifications, and emails will stop!"));
    addCheckBox(mDisableNotifications, u"Troubleshoot_Disable_Notifications"_s);

    mDisablePresenceBroadcast->setObjectName(u"mDisablePresenceBroadcast"_s);
    mDisablePresenceBroadcast->setToolTip(
        i18n("This setting prevents all instances from sending the status changes of the users to their clients keeping all the users with their presence "
             "status from the first load!"));
    addCheckBox(mDisablePresenceBroadcast, u"Troubleshoot_Disable_Presence_Broadcast"_s);

    mDisableInstanceBroadcast->setObjectName(u"mDisableInstanceBroadcast"_s);
    mDisableInstanceBroadcast->setToolTip(
        i18n("This setting prevents the Rocket.Chat instances from sending events to the other instances, it may cause syncing problems and misbehavior!"));
    addCheckBox(mDisableInstanceBroadcast, u"Troubleshoot_Disable_Instance_Broadcast"_s);

    mDisableSessionsMonitor->setObjectName(u"mDisableSessionsMonitor"_s);
    mDisableSessionsMonitor->setToolTip(
        i18n("This setting prevents the Rocket.Chat instances from sending events to the other instances, it may cause syncing problems and misbehavior!"));
    addCheckBox(mDisableSessionsMonitor, u"Troubleshoot_Disable_Sessions_Monitor"_s);

    mDisableLivechatActivityMonitor->setObjectName(u"mDisableLivechatActivityMonitor"_s);
    mDisableLivechatActivityMonitor->setToolTip(
        i18n("This setting stops the processing of livechat visitor sessions causing the statistics to stop working correctly!"));
    addCheckBox(mDisableLivechatActivityMonitor, u"Troubleshoot_Disable_Livechat_Activity_Monitor"_s);

    mDisableStatisticsGenerator->setObjectName(u"mDisableStatisticsGenerator"_s);
    mDisableStatisticsGenerator->setToolTip(
        i18n("This setting stops the processing all statistics making the info page outdated until someone clicks on the refresh button and may cause other "
             "missing information around the system!"));
    addCheckBox(mDisableStatisticsGenerator, u"Troubleshoot_Disable_Statistics_Generator"_s);

    mDisableDataExporterProcessor->setObjectName(u"mDisableDataExporterProcessor"_s);
    mDisableDataExporterProcessor->setToolTip(
        i18n("This setting stops the processing of all export requests from users, so they will not receive the link to download their data!"));
    addCheckBox(mDisableDataExporterProcessor, u"Troubleshoot_Disable_Data_Exporter_Processor"_s);

    mDisableWorkspaceSync->setObjectName(u"mDisableWorkspaceSync"_s);
    mDisableWorkspaceSync->setToolTip(
        i18n("This setting stops the sync of this server with Rocket.Chat's cloud and may cause issues with marketplace and enterprise licenses!"));
    addCheckBox(mDisableWorkspaceSync, u"Troubleshoot_Disable_Workspace_Sync"_s);

    mDisableTeamsMention->setObjectName(u"mDisableTeamsMention"_s);
    mDisableTeamsMention->setToolTip(
        i18n("This setting disables the teams mention feature. User's won't be able to mention a Team by name in a message and get its members notified."));
    addCheckBox(mDisableTeamsMention, u"Troubleshoot_Disable_Teams_Mention"_s);
}

TroubleshootSettingsWidget::~TroubleshootSettingsWidget() = default;

void TroubleshootSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
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
