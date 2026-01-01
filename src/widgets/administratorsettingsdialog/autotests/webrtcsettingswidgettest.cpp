/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "webrtcsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/webrtc/webrtcsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>

QTEST_MAIN(WebRtcSettingsWidgetTest)

WebRtcSettingsWidgetTest::WebRtcSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WebRtcSettingsWidgetTest::shouldHaveDefaultValues()
{
    WebRtcSettingsWidget w(nullptr);
    auto mEnabled = w.findChild<QCheckBox *>(u"mEnabled"_s);
    QVERIFY(mEnabled);
    QVERIFY(!mEnabled->isChecked());
    QVERIFY(!mEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnabled), u"WebRTC_Enabled"_s);

    auto mEnablePublicChannels = w.findChild<QCheckBox *>(u"mEnablePublicChannels"_s);
    QVERIFY(mEnablePublicChannels);
    QVERIFY(!mEnablePublicChannels->isChecked());
    QVERIFY(!mEnablePublicChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePublicChannels), u"WebRTC_Enable_Channel"_s);

    auto mEnablePrivateChannels = w.findChild<QCheckBox *>(u"mEnablePrivateChannels"_s);
    QVERIFY(mEnablePrivateChannels);
    QVERIFY(!mEnablePrivateChannels->isChecked());
    QVERIFY(!mEnablePrivateChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnablePrivateChannels), u"WebRTC_Enable_Private"_s);

    auto mEnableDirectMessages = w.findChild<QCheckBox *>(u"mEnableDirectMessages"_s);
    QVERIFY(mEnableDirectMessages);
    QVERIFY(!mEnableDirectMessages->isChecked());
    QVERIFY(!mEnableDirectMessages->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableDirectMessages), u"WebRTC_Enable_Direct"_s);

    auto mServer = w.findChild<QLineEdit *>(u"mServer"_s);
    QVERIFY(mServer);
    QVERIFY(mServer->text().isEmpty());
    QVERIFY(!mServer->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mServer), u"WebRTC_Servers"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"WebRTC_Servers"_s);
}

#include "moc_webrtcsettingswidgettest.cpp"
