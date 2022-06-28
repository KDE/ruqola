/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QSpinBox>

PasswordSettingsWidget::PasswordSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnablePasswordHistory(new QCheckBox(i18n("Enable Password History"), this))
    , mPasswordHistoryLength(new QSpinBox(this))
    , mEnablePasswordPolicy(new QCheckBox(i18n("Enable Password Policy"), this))
{
    // TODO add label
    mEnablePasswordHistory->setObjectName(QStringLiteral("mEnablePasswordHistory"));
    mEnablePasswordHistory->setToolTip(i18n("When enabled, users won't be able to update their passwords to some of their most recently used passwords."));
    mMainLayout->addWidget(mEnablePasswordHistory);
    connectCheckBox(mEnablePasswordHistory, QStringLiteral("Accounts_Password_History_Enabled"));

    mPasswordHistoryLength->setObjectName(QStringLiteral("mPasswordHistoryLength"));
    mPasswordHistoryLength->setToolTip(i18n("Amount of most recently used passwords to prevent users from reusing."));
    addSpinbox(i18n("Password History Length"), mPasswordHistoryLength, QStringLiteral("Accounts_Password_History_Amount"));

    // TODO add label
    mEnablePasswordPolicy->setObjectName(QStringLiteral("mEnablePasswordPolicy"));
    mEnablePasswordPolicy->setToolTip(i18n("When enabled, users won't be able to update their passwords to some of their most recently used passwords."));
    mMainLayout->addWidget(mEnablePasswordPolicy);
    connectCheckBox(mEnablePasswordPolicy, QStringLiteral("Accounts_Password_Policy_Enabled"));
}

PasswordSettingsWidget::~PasswordSettingsWidget() = default;

void PasswordSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnablePasswordHistory, mapSettings);
    initializeWidget(mPasswordHistoryLength, mapSettings);
    initializeWidget(mEnablePasswordPolicy, mapSettings);
}
