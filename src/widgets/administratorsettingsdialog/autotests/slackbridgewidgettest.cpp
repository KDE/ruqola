/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "slackbridgewidgettest.h"
#include "administratorsettingsdialog/slackbridge/slackbridgewidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QTest>
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
}
