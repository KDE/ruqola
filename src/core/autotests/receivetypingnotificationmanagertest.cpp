/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "receivetypingnotificationmanagertest.h"
#include "receivetypingnotificationmanager.h"
#include <QTest>
#include <QSignalSpy>

QTEST_GUILESS_MAIN(ReceiveTypingNotificationManagerTest)

ReceiveTypingNotificationManagerTest::ReceiveTypingNotificationManagerTest(QObject *parent)
    : QObject(parent)
{
}

void ReceiveTypingNotificationManagerTest::shouldHaveDefaultValues()
{
    ReceiveTypingNotificationManager manager;
    QVERIFY(manager.typingNotification(QStringLiteral("Foo")).isEmpty());
}

void ReceiveTypingNotificationManagerTest::shouldAddNotifications()
{
    ReceiveTypingNotificationManager manager;
    QSignalSpy spy(&manager, &ReceiveTypingNotificationManager::notificationChanged);

    manager.insertTypingNotification(QStringLiteral("room1"), QStringLiteral("user1"), false);
    QVERIFY(manager.typingNotification(QStringLiteral("room1")).isEmpty());
    QCOMPARE(spy.count(), 0);
    spy.clear();

    manager.insertTypingNotification(QStringLiteral("room1"), QStringLiteral("user1"), true);
    QCOMPARE(manager.typingNotification(QStringLiteral("room1")), QStringLiteral("user1 is typing..."));
    QCOMPARE(spy.count(), 1);
    manager.insertTypingNotification(QStringLiteral("room1"), QStringLiteral("user1"), true);
    QCOMPARE(manager.typingNotification(QStringLiteral("room1")), QStringLiteral("user1 is typing..."));
    QCOMPARE(spy.count(), 1);

    spy.clear();
    //Add different user.
    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user2"), true);
    QCOMPARE(manager.typingNotification(QStringLiteral("room2")), QStringLiteral("user2 is typing..."));
    QCOMPARE(spy.count(), 1);

    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user3"), true);
    QCOMPARE(manager.typingNotification(QStringLiteral("room2")), QStringLiteral("user2 and user3 are typing..."));
    QCOMPARE(spy.count(), 2);

    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user1"), true);
    QCOMPARE(manager.typingNotification(QStringLiteral("room2")), QStringLiteral("user2, user3 and user1 are typing..."));
    QCOMPARE(spy.count(), 3);

    //Remove typing
    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user3"), false);
    QCOMPARE(manager.typingNotification(QStringLiteral("room2")), QStringLiteral("user2 and user1 are typing..."));
    QCOMPARE(spy.count(), 4);

    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user1"), false);
    QCOMPARE(manager.typingNotification(QStringLiteral("room2")), QStringLiteral("user2 is typing..."));
    QCOMPARE(spy.count(), 5);

    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user3"), false);
    QCOMPARE(manager.typingNotification(QStringLiteral("room2")), QStringLiteral("user2 is typing..."));
    QCOMPARE(spy.count(), 5);

    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user2"), false);
    QVERIFY(manager.typingNotification(QStringLiteral("room2")).isEmpty());
    QCOMPARE(spy.count(), 6);

    //No user/roomId
    manager.insertTypingNotification(QStringLiteral("room2"), QStringLiteral("user2"), false);
    QVERIFY(manager.typingNotification(QStringLiteral("room2")).isEmpty());
    QCOMPARE(spy.count(), 6);

    spy.clear();
    QVERIFY(manager.typingNotification(QStringLiteral("room5")).isEmpty());
    QCOMPARE(spy.count(), 0);
}
