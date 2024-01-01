/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordsettingswidgettest.h"
#include "administratorsettingsdialog/password/passwordsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
QTEST_MAIN(PasswordSettingsWidgetTest)
PasswordSettingsWidgetTest::PasswordSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void PasswordSettingsWidgetTest::shouldHaveDefaultValues()
{
    PasswordSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto historyLabel = w.findChild<QLabel *>(QStringLiteral("historyLabel"));
    QVERIFY(historyLabel);
    QVERIFY(!historyLabel->text().isEmpty());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEnablePasswordHistory = w.findChild<QCheckBox *>(QStringLiteral("mEnablePasswordHistory"));
    QVERIFY(mEnablePasswordHistory);
    QVERIFY(!mEnablePasswordHistory->isChecked());
    QVERIFY(!mEnablePasswordHistory->text().isEmpty());
    QVERIFY(!mEnablePasswordHistory->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePasswordHistory), QStringLiteral("Accounts_Password_History_Enabled"));

    auto mPasswordHistoryLength = w.findChild<QSpinBox *>(QStringLiteral("mPasswordHistoryLength"));
    QVERIFY(mPasswordHistoryLength);
    QVERIFY(!mPasswordHistoryLength->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPasswordHistoryLength), QStringLiteral("Accounts_Password_History_Amount"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_Password_History_Amount"));

    auto mEnablePasswordPolicy = w.findChild<QCheckBox *>(QStringLiteral("mEnablePasswordPolicy"));
    QVERIFY(mEnablePasswordPolicy);
    QVERIFY(!mEnablePasswordPolicy->isChecked());
    QVERIFY(!mEnablePasswordPolicy->text().isEmpty());
    QVERIFY(!mEnablePasswordPolicy->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePasswordPolicy), QStringLiteral("Accounts_Password_Policy_Enabled"));

    auto mMinimumLength = w.findChild<QSpinBox *>(QStringLiteral("mMinimumLength"));
    QVERIFY(mMinimumLength);
    QVERIFY(!mMinimumLength->toolTip().isEmpty());
    QCOMPARE(mMinimumLength->minimum(), -1);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMinimumLength), QStringLiteral("Accounts_Password_Policy_MinLength"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_Password_Policy_MinLength"));

    auto mMaximumLength = w.findChild<QSpinBox *>(QStringLiteral("mMaximumLength"));
    QVERIFY(mMaximumLength);
    QVERIFY(!mMaximumLength->toolTip().isEmpty());
    QCOMPARE(mMaximumLength->minimum(), -1);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumLength), QStringLiteral("Accounts_Password_Policy_MaxLength"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_Password_Policy_MaxLength"));

    auto mForbidRepeatingCharacters = w.findChild<QCheckBox *>(QStringLiteral("mForbidRepeatingCharacters"));
    QVERIFY(mForbidRepeatingCharacters);
    QVERIFY(!mForbidRepeatingCharacters->isChecked());
    QVERIFY(!mForbidRepeatingCharacters->text().isEmpty());
    QVERIFY(!mForbidRepeatingCharacters->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForbidRepeatingCharacters), QStringLiteral("Accounts_Password_Policy_ForbidRepeatingCharacters"));

    auto mMaxRepeatingCharacters = w.findChild<QSpinBox *>(QStringLiteral("mMaxRepeatingCharacters"));
    QVERIFY(mMaxRepeatingCharacters);
    QVERIFY(!mMaxRepeatingCharacters->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaxRepeatingCharacters), QStringLiteral("Accounts_Password_Policy_ForbidRepeatingCharactersCount"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_Password_Policy_ForbidRepeatingCharactersCount"));

    auto mAtLeastOneLowercase = w.findChild<QCheckBox *>(QStringLiteral("mAtLeastOneLowercase"));
    QVERIFY(mAtLeastOneLowercase);
    QVERIFY(!mAtLeastOneLowercase->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneLowercase), QStringLiteral("Accounts_Password_Policy_AtLeastOneLowercase"));

    auto mAtLeastOneUppercase = w.findChild<QCheckBox *>(QStringLiteral("mAtLeastOneUppercase"));
    QVERIFY(mAtLeastOneUppercase);
    QVERIFY(!mAtLeastOneUppercase->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneUppercase), QStringLiteral("Accounts_Password_Policy_AtLeastOneUppercase"));

    auto mAtLeastOneNumber = w.findChild<QCheckBox *>(QStringLiteral("mAtLeastOneNumber"));
    QVERIFY(mAtLeastOneNumber);
    QVERIFY(!mAtLeastOneNumber->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneNumber), QStringLiteral("Accounts_Password_Policy_AtLeastOneNumber"));

    auto mAtLeastOneSymbol = w.findChild<QCheckBox *>(QStringLiteral("mAtLeastOneSymbol"));
    QVERIFY(mAtLeastOneSymbol);
    QVERIFY(!mAtLeastOneSymbol->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneSymbol), QStringLiteral("Accounts_Password_Policy_AtLeastOneSpecialCharacter"));

    auto passwordPolicyLabel = w.findChild<QLabel *>(QStringLiteral("passwordPolicyLabel"));
    QVERIFY(passwordPolicyLabel);
    QVERIFY(!passwordPolicyLabel->text().isEmpty());
}

#include "moc_passwordsettingswidgettest.cpp"
