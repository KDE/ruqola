/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurenotificationwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/configurenotificationwidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureNotificationWidgetTest)
ConfigureNotificationWidgetTest::ConfigureNotificationWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ConfigureNotificationWidgetTest::shouldHaveDefaultValues()
{
    ConfigureNotificationWidget w(nullptr);
    QVERIFY(!w.room());
    auto topLayout = w.findChild<QVBoxLayout *>(u"topLayout"_s);
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins{});

#if 0 // Failed. Don't know why . FIXME
    auto mDisableNotification = topLayout->findChild<QCheckBox *>(u"mDisableNotification"_s);
    QVERIFY(mDisableNotification);

    auto mHideUnreadRoomStatus = topLayout->findChild<QCheckBox *>(u"mHideUnreadRoomStatus"_s);
    QVERIFY(mHideUnreadRoomStatus);

    auto mMuteGroupMentions = topLayout->findChild<QCheckBox *>(u"mMuteGroupMentions"_s);
    QVERIFY(mMuteGroupMentions);

    auto desktopGroupBox = topLayout->findChild<QGroupBox *>(u"desktopGroupBox"_s);
    QVERIFY(desktopGroupBox);
    QVERIFY(!desktopGroupBox->title().isEmpty());

    auto desktopGroupBoxLayout = desktopGroupBox->findChild<QFormLayout *>(u"desktopGroupBoxLayout"_s);
    QVERIFY(desktopGroupBoxLayout);

    auto mDesktopAlertCombobox = topLayout->findChild<QComboBox *>(u"mDesktopAlertCombobox"_s);
    QVERIFY(mDesktopAlertCombobox);

    auto mDesktopAudioCombobox = topLayout->findChild<QComboBox *>(u"mDesktopAudioCombobox"_s);
    QVERIFY(mDesktopAudioCombobox);

    auto mDesktopSoundCombobox = topLayout->findChild<QComboBox *>(u"mDesktopSoundCombobox"_s);
    QVERIFY(mDesktopSoundCombobox);

    auto mPlaySoundToolButton = topLayout->findChild<QToolButton *>(u"mPlaySoundToolButton"_s);
    QVERIFY(mPlaySoundToolButton);

    auto mobileGroupBox = topLayout->findChild<QGroupBox *>(u"mobileGroupBox"_s);
    QVERIFY(mobileGroupBox);
    QVERIFY(!mobileGroupBox->title().isEmpty());

    auto mobileGroupBoxLayout = mobileGroupBox->findChild<QFormLayout *>(u"mobileGroupBoxLayout"_s);
    QVERIFY(mobileGroupBoxLayout);

    auto mMobileAlertCombobox = topLayout->findChild<QComboBox *>(u"mMobileAlertCombobox"_s);
    QVERIFY(mMobileAlertCombobox);

    auto emailGroupBox = topLayout->findChild<QGroupBox *>(u"emailGroupBox"_s);
    QVERIFY(emailGroupBox);
    QVERIFY(!emailGroupBox->title().isEmpty());

    auto emailGroupBoxLayout = emailGroupBox->findChild<QFormLayout *>(u"emailGroupBoxLayout"_s);
    QVERIFY(emailGroupBoxLayout);

    auto mEmailAlertCombobox = topLayout->findChild<QComboBox *>(u"mEmailAlertCombobox"_s);
    QVERIFY(mEmailAlertCombobox);
#endif
}

#include "moc_configurenotificationwidgettest.cpp"
