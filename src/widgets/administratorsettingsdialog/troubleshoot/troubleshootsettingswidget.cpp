/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
{
    mDisableNotifications->setObjectName(QStringLiteral("mDisableNotifications"));
    mMainLayout->addWidget(mDisableNotifications);
    mDisableNotifications->setToolTip(
        i18n("This setting completely disables the notifications system; sounds, desktop notifications, mobile notifications, and emails will stop!"));
    connectCheckBox(mDisableNotifications, QStringLiteral("Troubleshoot_Disable_Notifications"));

    mDisablePresenceBroadcast->setObjectName(QStringLiteral("mDisablePresenceBroadcast"));
    mMainLayout->addWidget(mDisablePresenceBroadcast);
    mDisablePresenceBroadcast->setToolTip(
        i18n("This setting prevents all instances form sending the status changes of the users to their clients keeping all the users with their presence "
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
}

TroubleshootSettingsWidget::~TroubleshootSettingsWidget() = default;

void TroubleshootSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mDisableNotifications, mapSettings, false);
    initializeWidget(mDisablePresenceBroadcast, mapSettings, false);
    initializeWidget(mDisableInstanceBroadcast, mapSettings, false);
    initializeWidget(mDisableSessionsMonitor, mapSettings, false);
}
