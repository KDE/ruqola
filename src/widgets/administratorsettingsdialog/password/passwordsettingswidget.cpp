/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFont>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>

PasswordSettingsWidget::PasswordSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnablePasswordHistory(new QCheckBox(i18nc("@option:check", "Enable Password History"), this))
    , mPasswordHistoryLength(new QSpinBox(this))
    , mEnablePasswordPolicy(new QCheckBox(i18nc("@option:check", "Enable Password Policy"), this))
    , mMinimumLength(new QSpinBox(this))
    , mMaximumLength(new QSpinBox(this))
    , mForbidRepeatingCharacters(new QCheckBox(i18nc("@option:check", "Forbid Repeating Characters"), this))
    , mMaxRepeatingCharacters(new QSpinBox(this))
    , mAtLeastOneLowercase(new QCheckBox(i18nc("@option:check", "At Least One Lowercase"), this))
    , mAtLeastOneUppercase(new QCheckBox(i18nc("@option:check", "At Least One Uppercase"), this))
    , mAtLeastOneNumber(new QCheckBox(i18nc("@option:check", "At Least One Number"), this))
    , mAtLeastOneSymbol(new QCheckBox(i18nc("@option:check", "At Least One Symbol"), this))
{
    auto historyLabel = createBoldLabel(i18n("Password History"));
    historyLabel->setObjectName(u"historyLabel"_s);
    mMainLayout->addWidget(historyLabel);

    mEnablePasswordHistory->setObjectName(u"mEnablePasswordHistory"_s);
    mEnablePasswordHistory->setToolTip(
        i18nc("@info:tooltip", "When enabled, users won't be able to update their passwords to some of their most recently used passwords."));
    addCheckBox(mEnablePasswordHistory, u"Accounts_Password_History_Enabled"_s);

    mPasswordHistoryLength->setObjectName(u"mPasswordHistoryLength"_s);
    mPasswordHistoryLength->setToolTip(i18nc("@info:tooltip", "Amount of most recently used passwords to prevent users from reusing."));
    addSpinbox(i18n("Password History Length"), mPasswordHistoryLength, u"Accounts_Password_History_Amount"_s);

    auto passwordPolicyLabel = createBoldLabel(i18n("Password Policy"));
    passwordPolicyLabel->setObjectName(u"passwordPolicyLabel"_s);
    mMainLayout->addWidget(passwordPolicyLabel);

    mEnablePasswordPolicy->setObjectName(u"mEnablePasswordPolicy"_s);
    mEnablePasswordPolicy->setToolTip(
        i18nc("@info:tooltip", "When enabled, users won't be able to update their passwords to some of their most recently used passwords."));
    addCheckBox(mEnablePasswordPolicy, u"Accounts_Password_Policy_Enabled"_s);

    mMinimumLength->setObjectName(u"mMinimumLength"_s);
    mMinimumLength->setToolTip(i18nc("@info:tooltip", "Ensures that passwords must have at least this amount of characters. Use -1 to disable."));
    mMinimumLength->setMinimum(-1);
    addSpinbox(i18n("Minimum Length"), mMinimumLength, u"Accounts_Password_Policy_MinLength"_s);

    mMaximumLength->setObjectName(u"mMaximumLength"_s);
    mMaximumLength->setToolTip(i18nc("@info:tooltip", "Ensures that passwords do not have more than this amount of characters. Use -1 to disable."));
    mMaximumLength->setMinimum(-1);
    addSpinbox(i18n("Maximum Length"), mMaximumLength, u"Accounts_Password_Policy_MaxLength"_s);

    mForbidRepeatingCharacters->setObjectName(u"mForbidRepeatingCharacters"_s);
    mForbidRepeatingCharacters->setToolTip(i18nc("@info:tooltip", "Ensures passwords do not contain the same character repeating next to each other."));
    addCheckBox(mForbidRepeatingCharacters, u"Accounts_Password_Policy_ForbidRepeatingCharacters"_s);

    mMaxRepeatingCharacters->setObjectName(u"mMaxRepeatingCharacters"_s);
    mMaxRepeatingCharacters->setToolTip(i18nc("@info:tooltip", "The amount of times a character can be repeating before it is not allowed."));
    addSpinbox(i18n("Max Repeating Characters"), mMaxRepeatingCharacters, u"Accounts_Password_Policy_ForbidRepeatingCharactersCount"_s);

    mAtLeastOneLowercase->setObjectName(u"mAtLeastOneLowercase"_s);
    mAtLeastOneLowercase->setToolTip(i18nc("@info:tooltip", "Enforce that a password contain at least one lowercase character."));
    addCheckBox(mAtLeastOneLowercase, u"Accounts_Password_Policy_AtLeastOneLowercase"_s);

    mAtLeastOneUppercase->setObjectName(u"mAtLeastOneUppercase"_s);
    mAtLeastOneUppercase->setToolTip(i18nc("@info:tooltip", "Enforce that a password contain at least one uppercase character."));
    addCheckBox(mAtLeastOneUppercase, u"Accounts_Password_Policy_AtLeastOneUppercase"_s);

    mAtLeastOneNumber->setObjectName(u"mAtLeastOneNumber"_s);
    mAtLeastOneNumber->setToolTip(i18nc("@info:tooltip", "Enforce that a password contain at least one numerical character."));
    addCheckBox(mAtLeastOneNumber, u"Accounts_Password_Policy_AtLeastOneNumber"_s);

    mAtLeastOneSymbol->setObjectName(u"mAtLeastOneSymbol"_s);
    mAtLeastOneSymbol->setToolTip(i18nc("@info:tooltip", "Enforce that a password contain at least one special character."));
    addCheckBox(mAtLeastOneSymbol, u"Accounts_Password_Policy_AtLeastOneSpecialCharacter"_s);

    connect(this, &PasswordSettingsWidget::changedDone, this, [this](const QString &variable) {
        if (variable == QLatin1String("Accounts_Password_History_Enabled")) {
            updateSettingsStatus(mEnablePasswordHistory->isChecked());
        }
    });
    connect(this, &PasswordSettingsWidget::changedCanceled, this, [this](const QString &variable) {
        if (variable == QLatin1String("Accounts_Password_History_Enabled")) {
            updateSettingsStatus(mEnablePasswordHistory->isChecked());
        }
    });
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

void PasswordSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnablePasswordHistory, mapSettings, false);
    initializeWidget(mPasswordHistoryLength, mapSettings, 5);
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
