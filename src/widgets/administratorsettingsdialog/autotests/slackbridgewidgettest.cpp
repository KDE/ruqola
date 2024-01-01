/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidgettest.h"
#include "administratorsettingsdialog/slackbridge/slackbridgewidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QPlainTextEdit>
QTEST_MAIN(SlackBridgeWidgetTest)
SlackBridgeWidgetTest::SlackBridgeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void SlackBridgeWidgetTest::shouldHaveDefaultValues()
{
    SlackBridgeWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QVERIFY(!mEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("SlackBridge_Enabled"));

    auto mFileUpload = w.findChild<QCheckBox *>(QStringLiteral("mFileUpload"));
    QVERIFY(mFileUpload);
    QVERIFY(!mFileUpload->isChecked());
    QVERIFY(!mFileUpload->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUpload), QStringLiteral("SlackBridge_FileUpload_Enabled"));

    auto mSlackBridgeOutEnabled = w.findChild<QCheckBox *>(QStringLiteral("mSlackBridgeOutEnabled"));
    QVERIFY(mSlackBridgeOutEnabled);
    QVERIFY(!mSlackBridgeOutEnabled->isChecked());
    QVERIFY(!mSlackBridgeOutEnabled->text().isEmpty());
    QVERIFY(!mSlackBridgeOutEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSlackBridgeOutEnabled), QStringLiteral("SlackBridge_Out_Enabled"));

    auto mSlackBridgeOutAll = w.findChild<QCheckBox *>(QStringLiteral("mSlackBridgeOutAll"));
    QVERIFY(mSlackBridgeOutAll);
    QVERIFY(!mSlackBridgeOutAll->isChecked());
    QVERIFY(!mSlackBridgeOutAll->text().isEmpty());
    QVERIFY(!mSlackBridgeOutAll->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSlackBridgeOutAll), QStringLiteral("SlackBridge_Out_All"));

    auto mAliasFormat = w.findChild<QLineEdit *>(QStringLiteral("mAliasFormat"));
    QVERIFY(mAliasFormat);
    QVERIFY(mAliasFormat->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAliasFormat), QStringLiteral("SlackBridge_AliasFormat"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SlackBridge_AliasFormat"));

    auto mExcludeBots = w.findChild<QLineEdit *>(QStringLiteral("mExcludeBots"));
    QVERIFY(mExcludeBots);
    QVERIFY(mExcludeBots->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mExcludeBots), QStringLiteral("SlackBridge_ExcludeBotnames"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SlackBridge_ExcludeBotnames"));

    auto mReactions = w.findChild<QCheckBox *>(QStringLiteral("mReactions"));
    QVERIFY(mReactions);
    QVERIFY(!mReactions->isChecked());
    QVERIFY(!mReactions->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mReactions), QStringLiteral("SlackBridge_Reactions_Enabled"));

    auto mAPITokens = w.findChild<QPlainTextEdit *>(QStringLiteral("mAPITokens"));
    QVERIFY(mAPITokens);
    QVERIFY(mAPITokens->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAPITokens), QStringLiteral("SlackBridge_APIToken"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SlackBridge_APIToken"));
}

#include "moc_slackbridgewidgettest.cpp"
