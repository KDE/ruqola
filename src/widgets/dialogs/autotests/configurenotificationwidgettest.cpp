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
    QVBoxLayout *topLayout = w.findChild<QVBoxLayout *>(QStringLiteral("topLayout"));
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QFormLayout *mainLayout = topLayout->findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));
    return; //TODO fix me


    QCheckBox *mDisableNotification = topLayout->findChild<QCheckBox *>(QStringLiteral("mDisableNotification"));
    QVERIFY(mDisableNotification);

    QCheckBox *mHideUnreadRoomStatus = topLayout->findChild<QCheckBox *>(QStringLiteral("mHideUnreadRoomStatus"));
    QVERIFY(mHideUnreadRoomStatus);

    QCheckBox *mMuteGroupMentions = topLayout->findChild<QCheckBox *>(QStringLiteral("mMuteGroupMentions"));
    QVERIFY(mMuteGroupMentions);

    QGroupBox *desktopGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("desktopGroupBox"));
    QVERIFY(desktopGroupBox);
    QVERIFY(!desktopGroupBox->title().isEmpty());

    QFormLayout *desktopGroupBoxLayout = desktopGroupBox->findChild<QFormLayout *>(QStringLiteral("desktopGroupBoxLayout"));
    QVERIFY(desktopGroupBoxLayout);


    QComboBox *mDesktopAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopAlertCombobox"));
    QVERIFY(mDesktopAlertCombobox);

    QComboBox *mDesktopAudioCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopAudioCombobox"));
    QVERIFY(mDesktopAudioCombobox);

    QComboBox *mDesktopSoundCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopSoundCombobox"));
    QVERIFY(mDesktopSoundCombobox);

    QComboBox *mDesktopDurationCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mDesktopDurationCombobox"));
    QVERIFY(mDesktopDurationCombobox);

    QGroupBox *mobileGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("mobileGroupBox"));
    QVERIFY(mobileGroupBox);
    QVERIFY(!mobileGroupBox->title().isEmpty());

    QFormLayout *mobileGroupBoxLayout = mobileGroupBox->findChild<QFormLayout *>(QStringLiteral("mobileGroupBoxLayout"));
    QVERIFY(mobileGroupBoxLayout);

    QComboBox *mMobileAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mMobileAlertCombobox"));
    QVERIFY(mMobileAlertCombobox);


    QGroupBox *emailGroupBox = topLayout->findChild<QGroupBox *>(QStringLiteral("emailGroupBox"));
    QVERIFY(emailGroupBox);
    QVERIFY(!emailGroupBox->title().isEmpty());

    QFormLayout *emailGroupBoxLayout = emailGroupBox->findChild<QFormLayout *>(QStringLiteral("emailGroupBoxLayout"));
    QVERIFY(emailGroupBoxLayout);

    QComboBox *mEmailAlertCombobox = topLayout->findChild<QComboBox *>(QStringLiteral("mEmailAlertCombobox"));
    QVERIFY(mEmailAlertCombobox);
}
