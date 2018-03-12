/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "notificationoptionstest.h"
#include "notificationoptions.h"
#include <QTest>
QTEST_GUILESS_MAIN(NotificationOptionsTest)

NotificationOptionsTest::NotificationOptionsTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationOptionsTest::shouldHaveDefaultValue()
{
    NotificationOptions w;
    QVERIFY(w.audioNotifications().isEmpty());
    QVERIFY(w.desktopNotifications().isEmpty());
    QVERIFY(w.mobilePushNotification().isEmpty());
    QVERIFY(w.emailNotifications().isEmpty());
    QVERIFY(w.unreadTrayIconAlert().isEmpty());
    QVERIFY(!w.disableNotifications());
    QVERIFY(!w.hideUnreadStatus());
    QVERIFY(w.audioNotificationValue().isEmpty());
    QCOMPARE(w.desktopNotificationDuration(), 0);
}

void NotificationOptionsTest::shouldAssignValue()
{
    NotificationOptions w;
    QString audioNotifications = QStringLiteral("foo");
    w.setAudioNotifications(audioNotifications);

    QString desktopNotifications = QStringLiteral("bla");
    w.setDesktopNotifications(desktopNotifications);

    QString mobilePushNotification = QStringLiteral("bli");
    w.setMobilePushNotification(mobilePushNotification);

    QString emailNotifications = QStringLiteral("blu");
    w.setEmailNotifications(emailNotifications);

    QString unreadTrayIconAlert = QStringLiteral("Ablu");
    w.setUnreadTrayIconAlert(unreadTrayIconAlert);

    QString audioNotificationValue = QStringLiteral("ZZZZ");
    w.setAudioNotificationValue(audioNotificationValue);

    bool disableNotifications = true;
    w.setDisableNotifications(disableNotifications);

    bool hideUnreadStatus = true;
    w.setHideUnreadStatus(hideUnreadStatus);

    int desktopNotificationDuration = 15;
    w.setDesktopNotificationDuration(desktopNotificationDuration);

    QCOMPARE(w.audioNotifications(), audioNotifications);
    QCOMPARE(w.desktopNotifications(), desktopNotifications);
    QCOMPARE(w.mobilePushNotification(), mobilePushNotification);
    QCOMPARE(w.emailNotifications(), emailNotifications);
    QCOMPARE(w.unreadTrayIconAlert(), unreadTrayIconAlert);
    QCOMPARE(w.audioNotificationValue(), audioNotificationValue);
    QCOMPARE(w.disableNotifications(), disableNotifications);
    QCOMPARE(w.hideUnreadStatus(), hideUnreadStatus);
    QCOMPARE(w.desktopNotificationDuration(), desktopNotificationDuration);

    NotificationOptions t;
    t = w;
    QCOMPARE(t, w);
}
