/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

PasswordSettingsWidget::PasswordSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnablePasswordHistory(new QCheckBox(i18n("Enable Password History"), this))
{
    mEnablePasswordHistory->setObjectName(QStringLiteral("mEnablePasswordHistory"));
    mEnablePasswordHistory->setToolTip(i18n("When enabled, users won't be able to update their passwords to some of their most recently used passwords."));
    mMainLayout->addWidget(mEnablePasswordHistory);
    connectCheckBox(mEnablePasswordHistory, QStringLiteral("Accounts_Password_History_Enabled"));
}

PasswordSettingsWidget::~PasswordSettingsWidget() = default;

void PasswordSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnablePasswordHistory, mapSettings);
}
