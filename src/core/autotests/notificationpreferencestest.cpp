/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationpreferencestest.h"
#include "notifications/notificationpreferences.h"
#include <QTest>
QTEST_GUILESS_MAIN(NotificationPreferencesTest)

NotificationPreferencesTest::NotificationPreferencesTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationPreferencesTest::shouldHaveDefaultValues()
{
    NotificationPreferences w;
    QVERIFY(w.emailNotificationModel());
    QVERIFY(w.mobileNotificationModel());
    QVERIFY(w.desktopNotificationModel());
    QVERIFY(w.desktopDurationNotificationModel());
    QVERIFY(w.desktopSoundNotificationModel());
    QVERIFY(!w.customSoundManager());
}

#include "moc_notificationpreferencestest.cpp"
