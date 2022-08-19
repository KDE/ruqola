/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit PasswordSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~PasswordSettingsWidget() override;
    void initialize(const QMap<QString, QVariant> &mapSettings) override;
    void initializeDefaultValue() override;

private:
    void updateSettingsStatus(bool enabled);
    QCheckBox *const mEnablePasswordHistory;
    QSpinBox *const mPasswordHistoryLength;
    QCheckBox *const mEnablePasswordPolicy;
    QSpinBox *const mMinimumLength;
    QSpinBox *const mMaximumLength;
    QCheckBox *const mForbidRepeatingCharacters;
    QSpinBox *const mMaxRepeatingCharacters;
    QCheckBox *const mAtLeastOneLowercase;
    QCheckBox *const mAtLeastOneUppercase;
    QCheckBox *const mAtLeastOneNumber;
    QCheckBox *const mAtLeastOneSymbol;
};
