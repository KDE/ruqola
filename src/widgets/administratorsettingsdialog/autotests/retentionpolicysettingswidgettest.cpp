/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentionpolicysettingswidgettest.h"
#include "administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QTest>

QTEST_MAIN(RetentionPolicySettingsWidgetTest)
RetentionPolicySettingsWidgetTest::RetentionPolicySettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RetentionPolicySettingsWidgetTest::shouldHaveDefaultValues()
{
    RetentionPolicySettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mEnabled), QStringLiteral("RetentionPolicy_Enabled"));

    auto mUseAdvancedRetentionPolicyConfiguration = w.findChild<QCheckBox *>(QStringLiteral("mUseAdvancedRetentionPolicyConfiguration"));
    QVERIFY(mUseAdvancedRetentionPolicyConfiguration);
    QVERIFY(!mUseAdvancedRetentionPolicyConfiguration->isChecked());
    QVERIFY(!mUseAdvancedRetentionPolicyConfiguration->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::checkBoxSettingsName(mUseAdvancedRetentionPolicyConfiguration), QStringLiteral("RetentionPolicy_Advanced_Precision"));
}
