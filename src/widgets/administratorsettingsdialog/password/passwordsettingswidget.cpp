/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFont>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

PasswordSettingsWidget::PasswordSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnablePasswordHistory(new QCheckBox(i18n("Enable Password History"), this))
    , mPasswordHistoryLength(new QSpinBox(this))
    , mEnablePasswordPolicy(new QCheckBox(i18n("Enable Password Policy"), this))
    , mMinimumLength(new QSpinBox(this))
    , mMaximumLength(new QSpinBox(this))
    , mForbidRepeatingCharacters(new QCheckBox(i18n("Forbid Repeating Characters"), this))
    , mMaxRepeatingCharacters(new QSpinBox(this))
    , mAtLeastOneLowercase(new QCheckBox(i18n("At Least One Lowercase"), this))
    , mAtLeastOneUppercase(new QCheckBox(i18n("At Least One Uppercase"), this))
    , mAtLeastOneNumber(new QCheckBox(i18n("At Least One Number"), this))
    , mAtLeastOneSymbol(new QCheckBox(i18n("At Least One Symbol"), this))
{
    auto historyLabel = createBoldLabel(i18n("Password History"));
    historyLabel->setObjectName(QStringLiteral("historyLabel"));
    mMainLayout->addWidget(historyLabel);

    mEnablePasswordHistory->setObjectName(QStringLiteral("mEnablePasswordHistory"));
    mEnablePasswordHistory->setToolTip(i18n("When enabled, users won't be able to update their passwords to some of their most recently used passwords."));
    mMainLayout->addWidget(mEnablePasswordHistory);
    connectCheckBox(mEnablePasswordHistory, QStringLiteral("Accounts_Password_History_Enabled"));

    mPasswordHistoryLength->setObjectName(QStringLiteral("mPasswordHistoryLength"));
    mPasswordHistoryLength->setToolTip(i18n("Amount of most recently used passwords to prevent users from reusing."));
    addSpinbox(i18n("Password History Length"), mPasswordHistoryLength, QStringLiteral("Accounts_Password_History_Amount"));

    auto passwordPolicyLabel = createBoldLabel(i18n("Password Policy"));
    passwordPolicyLabel->setObjectName(QStringLiteral("passwordPolicyLabel"));
    mMainLayout->addWidget(passwordPolicyLabel);

    mEnablePasswordPolicy->setObjectName(QStringLiteral("mEnablePasswordPolicy"));
    mEnablePasswordPolicy->setToolTip(i18n("When enabled, users won't be able to update their passwords to some of their most recently used passwords."));
    mMainLayout->addWidget(mEnablePasswordPolicy);
    connectCheckBox(mEnablePasswordPolicy, QStringLiteral("Accounts_Password_Policy_Enabled"));

    mMinimumLength->setObjectName(QStringLiteral("mMinimumLength"));
    mMinimumLength->setToolTip(i18n("Ensures that passwords must have at least this amount of characters. Use -1 to disable."));
    mMinimumLength->setMinimum(-1);
    addSpinbox(i18n("Minimum Length"), mMinimumLength, QStringLiteral("Accounts_Password_Policy_MinLength"));

    mMaximumLength->setObjectName(QStringLiteral("mMaximumLength"));
    mMaximumLength->setToolTip(i18n("Ensures that passwords do not have more than this amount of characters. Use -1 to disable."));
    mMaximumLength->setMinimum(-1);
    addSpinbox(i18n("Maximum Length"), mMaximumLength, QStringLiteral("Accounts_Password_Policy_MaxLength"));

    mForbidRepeatingCharacters->setObjectName(QStringLiteral("mForbidRepeatingCharacters"));
    mForbidRepeatingCharacters->setToolTip(i18n("Ensures passwords do not contain the same character repeating next to each other."));
    mMainLayout->addWidget(mForbidRepeatingCharacters);
    connectCheckBox(mForbidRepeatingCharacters, QStringLiteral("Accounts_Password_Policy_ForbidRepeatingCharacters"));

    mMaxRepeatingCharacters->setObjectName(QStringLiteral("mMaxRepeatingCharacters"));
    mMaxRepeatingCharacters->setToolTip(i18n("The amount of times a character can be repeating before it is not allowed."));
    addSpinbox(i18n("Max Repeating Characters"), mMaxRepeatingCharacters, QStringLiteral("Accounts_Password_Policy_ForbidRepeatingCharactersCount"));

    mAtLeastOneLowercase->setObjectName(QStringLiteral("mAtLeastOneLowercase"));
    mAtLeastOneLowercase->setToolTip(i18n("Enforce that a password contain at least one lowercase character."));
    mMainLayout->addWidget(mAtLeastOneLowercase);
    connectCheckBox(mAtLeastOneLowercase, QStringLiteral("Accounts_Password_Policy_AtLeastOneLowercase"));

    mAtLeastOneUppercase->setObjectName(QStringLiteral("mAtLeastOneUppercase"));
    mAtLeastOneUppercase->setToolTip(i18n("Enforce that a password contain at least one uppercase character."));
    mMainLayout->addWidget(mAtLeastOneUppercase);
    connectCheckBox(mAtLeastOneUppercase, QStringLiteral("Accounts_Password_Policy_AtLeastOneUppercase"));

    mAtLeastOneNumber->setObjectName(QStringLiteral("mAtLeastOneNumber"));
    mAtLeastOneNumber->setToolTip(i18n("Enforce that a password contain at least one numerical character."));
    mMainLayout->addWidget(mAtLeastOneNumber);
    connectCheckBox(mAtLeastOneNumber, QStringLiteral("Accounts_Password_Policy_AtLeastOneNumber"));

    mAtLeastOneSymbol->setObjectName(QStringLiteral("mAtLeastOneSymbol"));
    mAtLeastOneSymbol->setToolTip(i18n("Enforce that a password contain at least one special character."));
    mMainLayout->addWidget(mAtLeastOneSymbol);
    connectCheckBox(mAtLeastOneSymbol, QStringLiteral("Accounts_Password_Policy_AtLeastOneSpecialCharacter"));

    connect(mEnablePasswordHistory, &QCheckBox::clicked, this, &PasswordSettingsWidget::updateSettingsStatus);
}

PasswordSettingsWidget::~PasswordSettingsWidget() = default;

void PasswordSettingsWidget::updateSettingsStatus(bool enabled)
{
    mPasswordHistoryLength->setEnabled(enabled);
    mEnablePasswordPolicy->setEnabled(enabled);
    mMinimumLength->setEnabled(enabled);
    mMaximumLength->setEnabled(enabled);
    mForbidRepeatingCharacters->setEnabled(enabled);
    mMaxRepeatingCharacters->setEnabled(enabled);
    mAtLeastOneLowercase->setEnabled(enabled);
    mAtLeastOneUppercase->setEnabled(enabled);
    mAtLeastOneNumber->setEnabled(enabled);
    mAtLeastOneSymbol->setEnabled(enabled);
}

void PasswordSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnablePasswordHistory, mapSettings, false);
    initializeWidget(mPasswordHistoryLength, mapSettings, 7);
    initializeWidget(mEnablePasswordPolicy, mapSettings, true);
    initializeWidget(mMinimumLength, mapSettings, 7);
    initializeWidget(mMaximumLength, mapSettings, -1);
    initializeWidget(mForbidRepeatingCharacters, mapSettings, true);
    initializeWidget(mMaxRepeatingCharacters, mapSettings, 3);
    initializeWidget(mAtLeastOneLowercase, mapSettings, true);
    initializeWidget(mAtLeastOneUppercase, mapSettings, true);
    initializeWidget(mAtLeastOneNumber, mapSettings, true);
    initializeWidget(mAtLeastOneSymbol, mapSettings, true);
    updateSettingsStatus(false);
}

#include "moc_passwordsettingswidget.cpp"
