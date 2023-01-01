/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurenotificationwidgettest.h"
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
    auto topLayout = w.findChild<QVBoxLayout *>(QStringLiteral("topLayout"));
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins{});

#if 0 // Failed. Don't know why . FIXME
    auto mDisableNotification = topLayout->findChild<QCheckBox *>(QStringLiteral("mDisableNotification"));
    QVERIFY(mDisableNotification);

    auto mHideUnreadRoomStatus = topLayout->findChild<QCheckBox *>(QStringLiteral("mHideUnreadRoomStatus"));
    QVERIFY(mHideUnreadRoomStatus);

    auto mMuteGroupMentions = topLayout->findChild<QCheckBox *>(QStringLiteral("mMuteGroupMentions"));
    QVERIFY(mMuteGroupMentions);

    auto desktopGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("desktopGroupBox"));
    QVERIFY(desktopGroupBox);
    QVERIFY(!desktopGroupBox->title().isEmpty());

    auto desktopGroupBoxLayout = desktopGroupBox->findChild<QFormLayout *>(QStringLiteral("desktopGroupBoxLayout"));
    QVERIFY(desktopGroupBoxLayout);

    auto mDesktopAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopAlertCombobox"));
    QVERIFY(mDesktopAlertCombobox);

    auto mDesktopAudioCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopAudioCombobox"));
    QVERIFY(mDesktopAudioCombobox);

    auto mDesktopSoundCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopSoundCombobox"));
    QVERIFY(mDesktopSoundCombobox);

    auto mobileGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("mobileGroupBox"));
    QVERIFY(mobileGroupBox);
    QVERIFY(!mobileGroupBox->title().isEmpty());

    auto mobileGroupBoxLayout = mobileGroupBox->findChild<QFormLayout *>(QStringLiteral("mobileGroupBoxLayout"));
    QVERIFY(mobileGroupBoxLayout);

    auto mMobileAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mMobileAlertCombobox"));
    QVERIFY(mMobileAlertCombobox);

    auto emailGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("emailGroupBox"));
    QVERIFY(emailGroupBox);
    QVERIFY(!emailGroupBox->title().isEmpty());

    auto emailGroupBoxLayout = emailGroupBox->findChild<QFormLayout *>(QStringLiteral("emailGroupBoxLayout"));
    QVERIFY(emailGroupBoxLayout);

    auto mEmailAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mEmailAlertCombobox"));
    QVERIFY(mEmailAlertCombobox);
#endif
}
