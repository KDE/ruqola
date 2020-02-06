/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    ConfigureNotificationWidget w;
    QVERIFY(!w.roomWrapper());
    auto *topLayout = w.findChild<QVBoxLayout *>(QStringLiteral("topLayout"));
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *mainLayout = topLayout->findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    return; //TODO fix me


    auto *mDisableNotification = topLayout->findChild<QCheckBox *>(QStringLiteral("mDisableNotification"));
    QVERIFY(mDisableNotification);

    auto *mHideUnreadRoomStatus = topLayout->findChild<QCheckBox *>(QStringLiteral("mHideUnreadRoomStatus"));
    QVERIFY(mHideUnreadRoomStatus);

    auto *mMuteGroupMentions = topLayout->findChild<QCheckBox *>(QStringLiteral("mMuteGroupMentions"));
    QVERIFY(mMuteGroupMentions);

    auto *desktopGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("desktopGroupBox"));
    QVERIFY(desktopGroupBox);
    QVERIFY(!desktopGroupBox->title().isEmpty());

    auto *desktopGroupBoxLayout = desktopGroupBox->findChild<QFormLayout *>(QStringLiteral("desktopGroupBoxLayout"));
    QVERIFY(desktopGroupBoxLayout);


    auto *mDesktopAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopAlertCombobox"));
    QVERIFY(mDesktopAlertCombobox);

    auto *mDesktopAudioCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopAudioCombobox"));
    QVERIFY(mDesktopAudioCombobox);

    auto *mDesktopSoundCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopSoundCombobox"));
    QVERIFY(mDesktopSoundCombobox);

    auto *mDesktopDurationCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopDurationCombobox"));
    QVERIFY(mDesktopDurationCombobox);

    auto *mobileGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("mobileGroupBox"));
    QVERIFY(mobileGroupBox);
    QVERIFY(!mobileGroupBox->title().isEmpty());

    auto *mobileGroupBoxLayout = mobileGroupBox->findChild<QFormLayout *>(QStringLiteral("mobileGroupBoxLayout"));
    QVERIFY(mobileGroupBoxLayout);

    auto *mMobileAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mMobileAlertCombobox"));
    QVERIFY(mMobileAlertCombobox);


    auto *emailGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("emailGroupBox"));
    QVERIFY(emailGroupBox);
    QVERIFY(!emailGroupBox->title().isEmpty());

    auto *emailGroupBoxLayout = emailGroupBox->findChild<QFormLayout *>(QStringLiteral("emailGroupBoxLayout"));
    QVERIFY(emailGroupBoxLayout);

    auto *mEmailAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mEmailAlertCombobox"));
    QVERIFY(mEmailAlertCombobox);
}
