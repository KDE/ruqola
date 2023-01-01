/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "videoconferencewidgettest.h"
#include "administratorsettingsdialog/videoconference/videoconferencewidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(VideoConferenceWidgetTest)
VideoConferenceWidgetTest::VideoConferenceWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void VideoConferenceWidgetTest::shouldHaveDefaultValues()
{
    VideoConferenceWidget w(nullptr);
    auto bigBlueButtonLabel = w.findChild<QLabel *>(QStringLiteral("bigBlueButtonLabel"));
    QVERIFY(bigBlueButtonLabel);
    QVERIFY(!bigBlueButtonLabel->text().isEmpty());

    auto mBigBlueEnabled = w.findChild<QCheckBox *>(QStringLiteral("mBigBlueEnabled"));
    QVERIFY(mBigBlueEnabled);
    QVERIFY(!mBigBlueEnabled->isChecked());
    QVERIFY(!mBigBlueEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mBigBlueEnabled), QStringLiteral("bigbluebutton_Enabled"));

    auto mServer = w.findChild<QLineEdit *>(QStringLiteral("mServer"));
    QVERIFY(mServer);
    QVERIFY(mServer->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mServer), QStringLiteral("bigbluebutton_server"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("bigbluebutton_server"));

    auto mSharedSecret = w.findChild<QLineEdit *>(QStringLiteral("mSharedSecret"));
    QVERIFY(mSharedSecret);
    QVERIFY(mSharedSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSharedSecret), QStringLiteral("bigbluebutton_sharedSecret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("bigbluebutton_sharedSecret"));

    auto mAlwaysOpenNewWindow = w.findChild<QCheckBox *>(QStringLiteral("mAlwaysOpenNewWindow"));
    QVERIFY(mAlwaysOpenNewWindow);
    QVERIFY(!mAlwaysOpenNewWindow->isChecked());
    QVERIFY(!mAlwaysOpenNewWindow->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAlwaysOpenNewWindow), QStringLiteral("bigbluebutton_Open_New_Window"));

    auto mJitsiEnabled = w.findChild<QCheckBox *>(QStringLiteral("mJitsiEnabled"));
    QVERIFY(mJitsiEnabled);
    QVERIFY(!mJitsiEnabled->isChecked());
    QVERIFY(!mJitsiEnabled->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnabled), QStringLiteral("Jitsi_Enabled"));

    auto mJitsiDomain = w.findChild<QLineEdit *>(QStringLiteral("mJitsiDomain"));
    QVERIFY(mJitsiDomain);
    QVERIFY(mJitsiDomain->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiDomain), QStringLiteral("Jitsi_Domain"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Jitsi_Domain"));

    auto mJitsiRoomPrefix = w.findChild<QLineEdit *>(QStringLiteral("mJitsiRoomPrefix"));
    QVERIFY(mJitsiRoomPrefix);
    QVERIFY(mJitsiRoomPrefix->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiRoomPrefix), QStringLiteral("Jitsi_URL_Room_Prefix"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Jitsi_URL_Room_Prefix"));

    auto mJitsiRoomSuffix = w.findChild<QLineEdit *>(QStringLiteral("mJitsiRoomSuffix"));
    QVERIFY(mJitsiRoomSuffix);
    QVERIFY(mJitsiRoomSuffix->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiRoomSuffix), QStringLiteral("Jitsi_URL_Room_Suffix"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Jitsi_URL_Room_Suffix"));

    auto mEnableRoomNameHash = w.findChild<QCheckBox *>(QStringLiteral("mJitsiEnableRoomNameHash"));
    QVERIFY(mEnableRoomNameHash);
    QVERIFY(!mEnableRoomNameHash->isChecked());
    QVERIFY(!mEnableRoomNameHash->text().isEmpty());
    QVERIFY(!mEnableRoomNameHash->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableRoomNameHash), QStringLiteral("Jitsi_URL_Room_Hash"));

    auto mJitsiUseSSL = w.findChild<QCheckBox *>(QStringLiteral("mJitsiUseSSL"));
    QVERIFY(mJitsiUseSSL);
    QVERIFY(!mJitsiUseSSL->isChecked());
    QVERIFY(!mJitsiUseSSL->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiUseSSL), QStringLiteral("Jitsi_SSL"));

    auto mJitsiAlwaysOpenNewWindow = w.findChild<QCheckBox *>(QStringLiteral("mJitsiAlwaysOpenNewWindow"));
    QVERIFY(mJitsiAlwaysOpenNewWindow);
    QVERIFY(!mJitsiAlwaysOpenNewWindow->isChecked());
    QVERIFY(!mJitsiAlwaysOpenNewWindow->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiAlwaysOpenNewWindow), QStringLiteral("Jitsi_Open_New_Window"));

    auto mJitsiEnableInChannels = w.findChild<QCheckBox *>(QStringLiteral("mJitsiEnableInChannels"));
    QVERIFY(mJitsiEnableInChannels);
    QVERIFY(!mJitsiEnableInChannels->isChecked());
    QVERIFY(!mJitsiEnableInChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnableInChannels), QStringLiteral("Jitsi_Enable_Channels"));

    auto mJitsiEnableTeams = w.findChild<QCheckBox *>(QStringLiteral("mJitsiEnableTeams"));
    QVERIFY(mJitsiEnableTeams);
    QVERIFY(!mJitsiEnableTeams->isChecked());
    QVERIFY(!mJitsiEnableTeams->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnableTeams), QStringLiteral("Jitsi_Enable_Teams"));

    auto mJitsiChromeExtensionId = w.findChild<QLineEdit *>(QStringLiteral("mJitsiChromeExtensionId"));
    QVERIFY(mJitsiChromeExtensionId);
    QVERIFY(mJitsiChromeExtensionId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiChromeExtensionId), QStringLiteral("Jitsi_Chrome_Extension"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Jitsi_Chrome_Extension"));

    auto mJitsiEnableJWTAuth = w.findChild<QCheckBox *>(QStringLiteral("mJitsiEnableJWTAuth"));
    QVERIFY(mJitsiEnableJWTAuth);
    QVERIFY(!mJitsiEnableJWTAuth->isChecked());
    QVERIFY(!mJitsiEnableJWTAuth->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiEnableJWTAuth), QStringLiteral("Jitsi_Enabled_TokenAuth"));

    auto mJitsiApplicationID = w.findChild<QLineEdit *>(QStringLiteral("mJitsiApplicationID"));
    QVERIFY(mJitsiApplicationID);
    QVERIFY(mJitsiApplicationID->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiApplicationID), QStringLiteral("Jitsi_Application_ID"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Jitsi_Application_ID"));

    auto mJitsiApplicationSecret = w.findChild<QLineEdit *>(QStringLiteral("mJitsiApplicationSecret"));
    QVERIFY(mJitsiApplicationSecret);
    QVERIFY(mJitsiApplicationSecret->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiApplicationSecret), QStringLiteral("Jitsi_Application_Secret"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Jitsi_Application_Secret"));

    auto mJitsiLimitTokenJitsiRoom = w.findChild<QCheckBox *>(QStringLiteral("mJitsiLimitTokenJitsiRoom"));
    QVERIFY(mJitsiLimitTokenJitsiRoom);
    QVERIFY(!mJitsiLimitTokenJitsiRoom->isChecked());
    QVERIFY(!mJitsiLimitTokenJitsiRoom->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mJitsiLimitTokenJitsiRoom), QStringLiteral("Jitsi_Limit_Token_To_Room"));
}
