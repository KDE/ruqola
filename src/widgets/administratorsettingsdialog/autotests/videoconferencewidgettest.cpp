/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/videoconference/videoconferencewidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
QTEST_MAIN(VideoConferenceWidgetTest)
VideoConferenceWidgetTest::VideoConferenceWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceWidgetTest::shouldHaveDefaultValues()
{
    VideoConferenceWidget w(nullptr);
    auto bigBlueButtonLabel = w.findChild<QLabel *>(u"bigBlueButtonLabel"_s);
    QVERIFY(bigBlueButtonLabel);
    QVERIFY(!bigBlueButtonLabel->text().isEmpty());

    auto mBigBlueEnabled = w.findChild<QCheckBox *>(u"mBigBlueEnabled"_s);
    QVERIFY(mBigBlueEnabled);
    QVERIFY(!mBigBlueEnabled->isChecked());
    QVERIFY(!mBigBlueEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBigBlueEnabled), u"bigbluebutton_Enabled"_s);

    auto mServer = w.findChild<QLineEdit *>(u"mServer"_s);
    QVERIFY(mServer);
    QVERIFY(mServer->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mServer), u"bigbluebutton_server"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"bigbluebutton_server"_s);

    auto mSharedSecret = w.findChild<QLineEdit *>(u"mSharedSecret"_s);
    QVERIFY(mSharedSecret);
    QVERIFY(mSharedSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSharedSecret), u"bigbluebutton_sharedSecret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"bigbluebutton_sharedSecret"_s);

    auto mAlwaysOpenNewWindow = w.findChild<QCheckBox *>(u"mAlwaysOpenNewWindow"_s);
    QVERIFY(mAlwaysOpenNewWindow);
    QVERIFY(!mAlwaysOpenNewWindow->isChecked());
    QVERIFY(!mAlwaysOpenNewWindow->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAlwaysOpenNewWindow), u"bigbluebutton_Open_New_Window"_s);

    auto mJitsiEnabled = w.findChild<QCheckBox *>(u"mJitsiEnabled"_s);
    QVERIFY(mJitsiEnabled);
    QVERIFY(!mJitsiEnabled->isChecked());
    QVERIFY(!mJitsiEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnabled), u"Jitsi_Enabled"_s);

    auto mJitsiDomain = w.findChild<QLineEdit *>(u"mJitsiDomain"_s);
    QVERIFY(mJitsiDomain);
    QVERIFY(mJitsiDomain->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiDomain), u"Jitsi_Domain"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Jitsi_Domain"_s);

    auto mJitsiRoomPrefix = w.findChild<QLineEdit *>(u"mJitsiRoomPrefix"_s);
    QVERIFY(mJitsiRoomPrefix);
    QVERIFY(mJitsiRoomPrefix->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiRoomPrefix), u"Jitsi_URL_Room_Prefix"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Jitsi_URL_Room_Prefix"_s);

    auto mJitsiRoomSuffix = w.findChild<QLineEdit *>(u"mJitsiRoomSuffix"_s);
    QVERIFY(mJitsiRoomSuffix);
    QVERIFY(mJitsiRoomSuffix->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiRoomSuffix), u"Jitsi_URL_Room_Suffix"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Jitsi_URL_Room_Suffix"_s);

    auto mEnableRoomNameHash = w.findChild<QCheckBox *>(u"mJitsiEnableRoomNameHash"_s);
    QVERIFY(mEnableRoomNameHash);
    QVERIFY(!mEnableRoomNameHash->isChecked());
    QVERIFY(!mEnableRoomNameHash->text().isEmpty());
    QVERIFY(!mEnableRoomNameHash->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableRoomNameHash), u"Jitsi_URL_Room_Hash"_s);

    auto mJitsiUseSSL = w.findChild<QCheckBox *>(u"mJitsiUseSSL"_s);
    QVERIFY(mJitsiUseSSL);
    QVERIFY(!mJitsiUseSSL->isChecked());
    QVERIFY(!mJitsiUseSSL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiUseSSL), u"Jitsi_SSL"_s);

    auto mJitsiAlwaysOpenNewWindow = w.findChild<QCheckBox *>(u"mJitsiAlwaysOpenNewWindow"_s);
    QVERIFY(mJitsiAlwaysOpenNewWindow);
    QVERIFY(!mJitsiAlwaysOpenNewWindow->isChecked());
    QVERIFY(!mJitsiAlwaysOpenNewWindow->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiAlwaysOpenNewWindow), u"Jitsi_Open_New_Window"_s);

    auto mJitsiEnableInChannels = w.findChild<QCheckBox *>(u"mJitsiEnableInChannels"_s);
    QVERIFY(mJitsiEnableInChannels);
    QVERIFY(!mJitsiEnableInChannels->isChecked());
    QVERIFY(!mJitsiEnableInChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnableInChannels), u"Jitsi_Enable_Channels"_s);

    auto mJitsiEnableTeams = w.findChild<QCheckBox *>(u"mJitsiEnableTeams"_s);
    QVERIFY(mJitsiEnableTeams);
    QVERIFY(!mJitsiEnableTeams->isChecked());
    QVERIFY(!mJitsiEnableTeams->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnableTeams), u"Jitsi_Enable_Teams"_s);

    auto mJitsiChromeExtensionId = w.findChild<QLineEdit *>(u"mJitsiChromeExtensionId"_s);
    QVERIFY(mJitsiChromeExtensionId);
    QVERIFY(mJitsiChromeExtensionId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiChromeExtensionId), u"Jitsi_Chrome_Extension"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Jitsi_Chrome_Extension"_s);

    auto mJitsiEnableJWTAuth = w.findChild<QCheckBox *>(u"mJitsiEnableJWTAuth"_s);
    QVERIFY(mJitsiEnableJWTAuth);
    QVERIFY(!mJitsiEnableJWTAuth->isChecked());
    QVERIFY(!mJitsiEnableJWTAuth->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnableJWTAuth), u"Jitsi_Enabled_TokenAuth"_s);

    auto mJitsiApplicationID = w.findChild<QLineEdit *>(u"mJitsiApplicationID"_s);
    QVERIFY(mJitsiApplicationID);
    QVERIFY(mJitsiApplicationID->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiApplicationID), u"Jitsi_Application_ID"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Jitsi_Application_ID"_s);

    auto mJitsiApplicationSecret = w.findChild<QLineEdit *>(u"mJitsiApplicationSecret"_s);
    QVERIFY(mJitsiApplicationSecret);
    QVERIFY(mJitsiApplicationSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiApplicationSecret), u"Jitsi_Application_Secret"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Jitsi_Application_Secret"_s);

    auto mJitsiLimitTokenJitsiRoom = w.findChild<QCheckBox *>(u"mJitsiLimitTokenJitsiRoom"_s);
    QVERIFY(mJitsiLimitTokenJitsiRoom);
    QVERIFY(!mJitsiLimitTokenJitsiRoom->isChecked());
    QVERIFY(!mJitsiLimitTokenJitsiRoom->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiLimitTokenJitsiRoom), u"Jitsi_Limit_Token_To_Room"_s);
}

#include "moc_videoconferencewidgettest.cpp"
