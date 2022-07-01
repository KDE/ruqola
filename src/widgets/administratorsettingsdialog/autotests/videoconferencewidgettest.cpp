/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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
}
