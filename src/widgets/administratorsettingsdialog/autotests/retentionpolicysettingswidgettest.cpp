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
}
