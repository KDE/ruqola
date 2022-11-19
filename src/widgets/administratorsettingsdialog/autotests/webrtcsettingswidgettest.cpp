/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webrtcsettingswidgettest.h"
#include "administratorsettingsdialog/webrtc/webrtcsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>

QTEST_MAIN(WebRtcSettingsWidgetTest)

WebRtcSettingsWidgetTest::WebRtcSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WebRtcSettingsWidgetTest::shouldHaveDefaultValues()
{
    WebRtcSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(QStringLiteral("mEnabled"));
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), QStringLiteral("WebRTC_Enabled"));

    auto mEnablePublicChannels = w.findChild<QCheckBox *>(QStringLiteral("mEnablePublicChannels"));
    QVERIFY(mEnablePublicChannels);
    QVERIFY(!mEnablePublicChannels->isChecked());
    QVERIFY(!mEnablePublicChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePublicChannels), QStringLiteral("WebRTC_Enable_Channel"));

    auto mEnablePrivateChannels = w.findChild<QCheckBox *>(QStringLiteral("mEnablePrivateChannels"));
    QVERIFY(mEnablePrivateChannels);
    QVERIFY(!mEnablePrivateChannels->isChecked());
    QVERIFY(!mEnablePrivateChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePrivateChannels), QStringLiteral("WebRTC_Enable_Private"));

    auto mEnableDirectMessages = w.findChild<QCheckBox *>(QStringLiteral("mEnableDirectMessages"));
    QVERIFY(mEnableDirectMessages);
    QVERIFY(!mEnableDirectMessages->isChecked());
    QVERIFY(!mEnableDirectMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableDirectMessages), QStringLiteral("WebRTC_Enable_Direct"));

    auto mServer = w.findChild<QLineEdit *>(QStringLiteral("mServer"));
    QVERIFY(mServer);
    QVERIFY(mServer->text().isEmpty());
    QVERIFY(!mServer->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mServer), QStringLiteral("WebRTC_Servers"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("WebRTC_Servers"));
}
