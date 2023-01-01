/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationtest.h"
#include "notifications/notification.h"
#include <QTest>
QTEST_MAIN(NotificationTest)

NotificationTest::NotificationTest(QObject *parent)
    : QObject(parent)
{
}

void NotificationTest::shouldHaveDefaultValue()
{
    Notification w;
    QCOMPARE(w.status(), KStatusNotifierItem::Passive);
}

void NotificationTest::shouldChangeStatus()
{
    Notification w;
    // Notification
    w.updateNotification(true, 3, QStringLiteral("foo"));
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    // Clear it.
    w.updateNotification(false, 0, QStringLiteral("foo"));
    QCOMPARE(w.status(), KStatusNotifierItem::Passive);

    w.updateNotification(false, 0, QStringLiteral("Bla"));
    QCOMPARE(w.status(), KStatusNotifierItem::Passive);
    w.updateNotification(true, 0, QStringLiteral("Bla"));
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    w.updateNotification(true, 3, QStringLiteral("Bla"));
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    w.updateNotification(false, 3, QStringLiteral("Bla"));
    QCOMPARE(w.status(), KStatusNotifierItem::Active);
    w.updateNotification(false, 0, QStringLiteral("Bla"));
    QCOMPARE(w.status(), KStatusNotifierItem::Passive);
}
