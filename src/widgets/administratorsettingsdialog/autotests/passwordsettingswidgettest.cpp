/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto historyLabel = w.findChild<QLabel *>(u"historyLabel"_s);
    QVERIFY(historyLabel);
    QVERIFY(!historyLabel->text().isEmpty());

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mEnablePasswordHistory = w.findChild<QCheckBox *>(u"mEnablePasswordHistory"_s);
    QVERIFY(mEnablePasswordHistory);
    QVERIFY(!mEnablePasswordHistory->isChecked());
    QVERIFY(!mEnablePasswordHistory->text().isEmpty());
    QVERIFY(!mEnablePasswordHistory->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePasswordHistory), u"Accounts_Password_History_Enabled"_s);

    auto mPasswordHistoryLength = w.findChild<QSpinBox *>(u"mPasswordHistoryLength"_s);
    QVERIFY(mPasswordHistoryLength);
    QVERIFY(!mPasswordHistoryLength->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mPasswordHistoryLength), u"Accounts_Password_History_Amount"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_Password_History_Amount"_s);

    auto mEnablePasswordPolicy = w.findChild<QCheckBox *>(u"mEnablePasswordPolicy"_s);
    QVERIFY(mEnablePasswordPolicy);
    QVERIFY(!mEnablePasswordPolicy->isChecked());
    QVERIFY(!mEnablePasswordPolicy->text().isEmpty());
    QVERIFY(!mEnablePasswordPolicy->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePasswordPolicy), u"Accounts_Password_Policy_Enabled"_s);

    auto mMinimumLength = w.findChild<QSpinBox *>(u"mMinimumLength"_s);
    QVERIFY(mMinimumLength);
    QVERIFY(!mMinimumLength->toolTip().isEmpty());
    QCOMPARE(mMinimumLength->minimum(), -1);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMinimumLength), u"Accounts_Password_Policy_MinLength"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_Password_Policy_MinLength"_s);

    auto mMaximumLength = w.findChild<QSpinBox *>(u"mMaximumLength"_s);
    QVERIFY(mMaximumLength);
    QVERIFY(!mMaximumLength->toolTip().isEmpty());
    QCOMPARE(mMaximumLength->minimum(), -1);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumLength), u"Accounts_Password_Policy_MaxLength"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_Password_Policy_MaxLength"_s);

    auto mForbidRepeatingCharacters = w.findChild<QCheckBox *>(u"mForbidRepeatingCharacters"_s);
    QVERIFY(mForbidRepeatingCharacters);
    QVERIFY(!mForbidRepeatingCharacters->isChecked());
    QVERIFY(!mForbidRepeatingCharacters->text().isEmpty());
    QVERIFY(!mForbidRepeatingCharacters->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mForbidRepeatingCharacters), u"Accounts_Password_Policy_ForbidRepeatingCharacters"_s);

    auto mMaxRepeatingCharacters = w.findChild<QSpinBox *>(u"mMaxRepeatingCharacters"_s);
    QVERIFY(mMaxRepeatingCharacters);
    QVERIFY(!mMaxRepeatingCharacters->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaxRepeatingCharacters), u"Accounts_Password_Policy_ForbidRepeatingCharactersCount"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Accounts_Password_Policy_ForbidRepeatingCharactersCount"_s);

    auto mAtLeastOneLowercase = w.findChild<QCheckBox *>(u"mAtLeastOneLowercase"_s);
    QVERIFY(mAtLeastOneLowercase);
    QVERIFY(!mAtLeastOneLowercase->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneLowercase), u"Accounts_Password_Policy_AtLeastOneLowercase"_s);

    auto mAtLeastOneUppercase = w.findChild<QCheckBox *>(u"mAtLeastOneUppercase"_s);
    QVERIFY(mAtLeastOneUppercase);
    QVERIFY(!mAtLeastOneUppercase->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneUppercase), u"Accounts_Password_Policy_AtLeastOneUppercase"_s);

    auto mAtLeastOneNumber = w.findChild<QCheckBox *>(u"mAtLeastOneNumber"_s);
    QVERIFY(mAtLeastOneNumber);
    QVERIFY(!mAtLeastOneNumber->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneNumber), u"Accounts_Password_Policy_AtLeastOneNumber"_s);

    auto mAtLeastOneSymbol = w.findChild<QCheckBox *>(u"mAtLeastOneSymbol"_s);
    QVERIFY(mAtLeastOneSymbol);
    QVERIFY(!mAtLeastOneSymbol->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAtLeastOneSymbol), u"Accounts_Password_Policy_AtLeastOneSpecialCharacter"_s);

    auto passwordPolicyLabel = w.findChild<QLabel *>(u"passwordPolicyLabel"_s);
    QVERIFY(passwordPolicyLabel);
    QVERIFY(!passwordPolicyLabel->text().isEmpty());
}

#include "moc_passwordsettingswidgettest.cpp"
