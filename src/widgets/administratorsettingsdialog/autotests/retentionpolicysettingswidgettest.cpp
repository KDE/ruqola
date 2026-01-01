/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "retentionpolicysettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

QTEST_MAIN(RetentionPolicySettingsWidgetTest)
RetentionPolicySettingsWidgetTest::RetentionPolicySettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RetentionPolicySettingsWidgetTest::shouldHaveDefaultValues()
{
    RetentionPolicySettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mEnabled = w.findChild<QCheckBox *>(u"mEnabled"_s);
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), u"RetentionPolicy_Enabled"_s);

    auto mUseAdvancedRetentionPolicyConfiguration = w.findChild<QCheckBox *>(u"mUseAdvancedRetentionPolicyConfiguration"_s);
    QVERIFY(mUseAdvancedRetentionPolicyConfiguration);
    QVERIFY(!mUseAdvancedRetentionPolicyConfiguration->isChecked());
    QVERIFY(!mUseAdvancedRetentionPolicyConfiguration->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseAdvancedRetentionPolicyConfiguration), u"RetentionPolicy_Advanced_Precision"_s);

    auto mDontPruneThreads = w.findChild<QCheckBox *>(u"mDontPruneThreads"_s);
    QVERIFY(mDontPruneThreads);
    QVERIFY(!mDontPruneThreads->isChecked());
    QVERIFY(!mDontPruneThreads->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDontPruneThreads), u"RetentionPolicy_DoNotPruneThreads"_s);

    auto mDontPruneDiscussion = w.findChild<QCheckBox *>(u"mDontPruneDiscussion"_s);
    QVERIFY(mDontPruneDiscussion);
    QVERIFY(!mDontPruneDiscussion->isChecked());
    QVERIFY(!mDontPruneDiscussion->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDontPruneDiscussion), u"RetentionPolicy_DoNotPruneDiscussion"_s);

    auto mDontPrunePinnedMessages = w.findChild<QCheckBox *>(u"mDontPrunePinnedMessages"_s);
    QVERIFY(mDontPrunePinnedMessages);
    QVERIFY(!mDontPrunePinnedMessages->isChecked());
    QVERIFY(!mDontPrunePinnedMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDontPrunePinnedMessages), u"RetentionPolicy_DoNotPrunePinned"_s);

    auto mOnlyDeleteFiles = w.findChild<QCheckBox *>(u"mOnlyDeleteFiles"_s);
    QVERIFY(mOnlyDeleteFiles);
    QVERIFY(!mOnlyDeleteFiles->isChecked());
    QVERIFY(!mOnlyDeleteFiles->text().isEmpty());
    QVERIFY(!mOnlyDeleteFiles->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mOnlyDeleteFiles), u"RetentionPolicy_FilesOnly"_s);

    auto mAppliesChannels = w.findChild<QCheckBox *>(u"mAppliesChannels"_s);
    QVERIFY(mAppliesChannels);
    QVERIFY(!mAppliesChannels->isChecked());
    QVERIFY(!mAppliesChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAppliesChannels), u"RetentionPolicy_AppliesToChannels"_s);

    auto mAppliesGroups = w.findChild<QCheckBox *>(u"mAppliesGroups"_s);
    QVERIFY(mAppliesGroups);
    QVERIFY(!mAppliesGroups->isChecked());
    QVERIFY(!mAppliesGroups->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAppliesGroups), u"RetentionPolicy_AppliesToGroups"_s);

    auto mAppliesMessages = w.findChild<QCheckBox *>(u"mAppliesMessages"_s);
    QVERIFY(mAppliesMessages);
    QVERIFY(!mAppliesMessages->isChecked());
    QVERIFY(!mAppliesMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAppliesMessages), u"RetentionPolicy_AppliesToDMs"_s);

    auto mMaximumMessageDirectMessages = w.findChild<QSpinBox *>(u"mMaximumMessageDirectMessages"_s);
    QVERIFY(mMaximumMessageDirectMessages);
    QVERIFY(!mMaximumMessageDirectMessages->toolTip().isEmpty());
    QCOMPARE(mMaximumMessageDirectMessages->maximum(), 9999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumMessageDirectMessages), u"RetentionPolicy_MaxAge_DMs"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"RetentionPolicy_MaxAge_DMs"_s);

    auto mMaximumMessagePrivateGroups = w.findChild<QSpinBox *>(u"mMaximumMessagePrivateGroups"_s);
    QVERIFY(mMaximumMessagePrivateGroups);
    QVERIFY(!mMaximumMessagePrivateGroups->toolTip().isEmpty());
    QCOMPARE(mMaximumMessagePrivateGroups->maximum(), 9999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumMessagePrivateGroups), u"RetentionPolicy_MaxAge_Groups"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"RetentionPolicy_MaxAge_Groups"_s);

    auto mMaximumMessageChannels = w.findChild<QSpinBox *>(u"mMaximumMessageChannels"_s);
    QVERIFY(mMaximumMessageChannels);
    QVERIFY(!mMaximumMessageChannels->toolTip().isEmpty());
    QCOMPARE(mMaximumMessageChannels->maximum(), 9999);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaximumMessageChannels), u"RetentionPolicy_MaxAge_Channels"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"RetentionPolicy_MaxAge_Channels"_s);

    auto mTimerPrecision = w.findChild<QComboBox *>(u"mTimerPrecision"_s);
    QVERIFY(mTimerPrecision);
    QCOMPARE(mTimerPrecision->count(), 4);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mTimerPrecision), u"RetentionPolicy_Precision"_s);
}

#include "moc_retentionpolicysettingswidgettest.cpp"
