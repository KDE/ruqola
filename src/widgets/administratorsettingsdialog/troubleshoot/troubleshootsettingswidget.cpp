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
{
    mDisableNotifications->setObjectName(QStringLiteral("mDisableNotifications"));
    mMainLayout->addWidget(mDisableNotifications);
    mDisableNotifications->setToolTip(
        i18n("This setting completely disables the notifications system; sounds, desktop notifications, mobile notifications, and emails will stop!"));
    connectCheckBox(mDisableNotifications, QStringLiteral("Troubleshoot_Disable_Notifications"));
}

TroubleshootSettingsWidget::~TroubleshootSettingsWidget() = default;

void TroubleshootSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mDisableNotifications, mapSettings, false);
}
