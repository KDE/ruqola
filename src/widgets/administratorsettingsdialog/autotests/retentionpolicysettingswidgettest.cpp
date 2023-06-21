/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentionpolicysettingswidgettest.h"
#include "administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(RetentionPolicySettingsWidgetTest)
RetentionPolicySettingsWidgetTest::RetentionPolicySettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RetentionPolicySettingsWidgetTest::shouldHaveDefaultValues()
{
    RetentionPolicySettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("RetentionPolicy_Enabled"));

    auto mUseAdvancedRetentionPolicyConfiguration = w.findChild<QCheckBox *>(QStringLiteral("mUseAdvancedRetentionPolicyConfiguration"));
    QVERIFY(mUseAdvancedRetentionPolicyConfiguration);
    QVERIFY(!mUseAdvancedRetentionPolicyConfiguration->isChecked());
    QVERIFY(!mUseAdvancedRetentionPolicyConfiguration->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseAdvancedRetentionPolicyConfiguration), QStringLiteral("RetentionPolicy_Advanced_Precision"));

    auto mDontPruneThreads = w.findChild<QCheckBox *>(QStringLiteral("mDontPruneThreads"));
    QVERIFY(mDontPruneThreads);
    QVERIFY(!mDontPruneThreads->isChecked());
    QVERIFY(!mDontPruneThreads->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDontPruneThreads), QStringLiteral("RetentionPolicy_DoNotPruneThreads"));

    auto mDontPruneDiscussion = w.findChild<QCheckBox *>(QStringLiteral("mDontPruneDiscussion"));
    QVERIFY(mDontPruneDiscussion);
    QVERIFY(!mDontPruneDiscussion->isChecked());
    QVERIFY(!mDontPruneDiscussion->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDontPruneDiscussion), QStringLiteral("RetentionPolicy_DoNotPruneDiscussion"));

    auto mDontPrunePinnedMessages = w.findChild<QCheckBox *>(QStringLiteral("mDontPrunePinnedMessages"));
    QVERIFY(mDontPrunePinnedMessages);
    QVERIFY(!mDontPrunePinnedMessages->isChecked());
    QVERIFY(!mDontPrunePinnedMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDontPrunePinnedMessages), QStringLiteral("RetentionPolicy_DoNotPrunePinned"));

    auto mOnlyDeleteFiles = w.findChild<QCheckBox *>(QStringLiteral("mOnlyDeleteFiles"));
    QVERIFY(mOnlyDeleteFiles);
    QVERIFY(!mOnlyDeleteFiles->isChecked());
    QVERIFY(!mOnlyDeleteFiles->text().isEmpty());
    QVERIFY(!mOnlyDeleteFiles->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mOnlyDeleteFiles), QStringLiteral("RetentionPolicy_FilesOnly"));

    auto mAppliesChannels = w.findChild<QCheckBox *>(QStringLiteral("mAppliesChannels"));
    QVERIFY(mAppliesChannels);
    QVERIFY(!mAppliesChannels->isChecked());
    QVERIFY(!mAppliesChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAppliesChannels), QStringLiteral("RetentionPolicy_AppliesToChannels"));

    auto mAppliesGroups = w.findChild<QCheckBox *>(QStringLiteral("mAppliesGroups"));
    QVERIFY(mAppliesGroups);
    QVERIFY(!mAppliesGroups->isChecked());
    QVERIFY(!mAppliesGroups->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAppliesGroups), QStringLiteral("RetentionPolicy_AppliesToGroups"));

    auto mAppliesMessages = w.findChild<QCheckBox *>(QStringLiteral("mAppliesMessages"));
    QVERIFY(mAppliesMessages);
    QVERIFY(!mAppliesMessages->isChecked());
    QVERIFY(!mAppliesMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAppliesMessages), QStringLiteral("RetentionPolicy_AppliesToDMs"));

    auto mMaximumMessageDirectMessages = w.findChild<QSpinBox *>(QStringLiteral("mMaximumMessageDirectMessages"));
    QVERIFY(mMaximumMessageDirectMessages);
    QVERIFY(!mMaximumMessageDirectMessages->toolTip().isEmpty());
    QCOMPARE(mMaximumMessageDirectMessages->maximum(), 9999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumMessageDirectMessages), QStringLiteral("RetentionPolicy_MaxAge_DMs"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("RetentionPolicy_MaxAge_DMs"));

    auto mMaximumMessagePrivateGroups = w.findChild<QSpinBox *>(QStringLiteral("mMaximumMessagePrivateGroups"));
    QVERIFY(mMaximumMessagePrivateGroups);
    QVERIFY(!mMaximumMessagePrivateGroups->toolTip().isEmpty());
    QCOMPARE(mMaximumMessagePrivateGroups->maximum(), 9999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumMessagePrivateGroups), QStringLiteral("RetentionPolicy_MaxAge_Groups"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("RetentionPolicy_MaxAge_Groups"));

    auto mMaximumMessageChannels = w.findChild<QSpinBox *>(QStringLiteral("mMaximumMessageChannels"));
    QVERIFY(mMaximumMessageChannels);
    QVERIFY(!mMaximumMessageChannels->toolTip().isEmpty());
    QCOMPARE(mMaximumMessageChannels->maximum(), 9999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumMessageChannels), QStringLiteral("RetentionPolicy_MaxAge_Channels"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("RetentionPolicy_MaxAge_Channels"));

    auto mTimerPrecision = w.findChild<QComboBox *>(QStringLiteral("mTimerPrecision"));
    QVERIFY(mTimerPrecision);
    QCOMPARE(mTimerPrecision->count(), 4);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTimerPrecision), QStringLiteral("RetentionPolicy_Precision"));
}

#include "moc_retentionpolicysettingswidgettest.cpp"
