/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mEnabled = w.findChild<QCheckBox *>(u"mEnabled"_s);
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QVERIFY(!mEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), u"SlackBridge_Enabled"_s);

    auto mFileUpload = w.findChild<QCheckBox *>(u"mFileUpload"_s);
    QVERIFY(mFileUpload);
    QVERIFY(!mFileUpload->isChecked());
    QVERIFY(!mFileUpload->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFileUpload), u"SlackBridge_FileUpload_Enabled"_s);

    auto mSlackBridgeOutEnabled = w.findChild<QCheckBox *>(u"mSlackBridgeOutEnabled"_s);
    QVERIFY(mSlackBridgeOutEnabled);
    QVERIFY(!mSlackBridgeOutEnabled->isChecked());
    QVERIFY(!mSlackBridgeOutEnabled->text().isEmpty());
    QVERIFY(!mSlackBridgeOutEnabled->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSlackBridgeOutEnabled), u"SlackBridge_Out_Enabled"_s);

    auto mSlackBridgeOutAll = w.findChild<QCheckBox *>(u"mSlackBridgeOutAll"_s);
    QVERIFY(mSlackBridgeOutAll);
    QVERIFY(!mSlackBridgeOutAll->isChecked());
    QVERIFY(!mSlackBridgeOutAll->text().isEmpty());
    QVERIFY(!mSlackBridgeOutAll->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSlackBridgeOutAll), u"SlackBridge_Out_All"_s);

    auto mAliasFormat = w.findChild<QLineEdit *>(u"mAliasFormat"_s);
    QVERIFY(mAliasFormat);
    QVERIFY(mAliasFormat->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAliasFormat), u"SlackBridge_AliasFormat"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"SlackBridge_AliasFormat"_s);

    auto mExcludeBots = w.findChild<QLineEdit *>(u"mExcludeBots"_s);
    QVERIFY(mExcludeBots);
    QVERIFY(mExcludeBots->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mExcludeBots), u"SlackBridge_ExcludeBotnames"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"SlackBridge_ExcludeBotnames"_s);

    auto mReactions = w.findChild<QCheckBox *>(u"mReactions"_s);
    QVERIFY(mReactions);
    QVERIFY(!mReactions->isChecked());
    QVERIFY(!mReactions->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mReactions), u"SlackBridge_Reactions_Enabled"_s);

    auto mAPITokens = w.findChild<QPlainTextEdit *>(u"mAPITokens"_s);
    QVERIFY(mAPITokens);
    QVERIFY(mAPITokens->toPlainText().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAPITokens), u"SlackBridge_APIToken"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"SlackBridge_APIToken"_s);
}

#include "moc_slackbridgewidgettest.cpp"
