/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "receivetypingnotificationmanagertest.h"
using namespace Qt::Literals::StringLiterals;

#include "receivetypingnotificationmanager.h"
#include <QSignalSpy>
#include <QTest>

QTEST_GUILESS_MAIN(ReceiveTypingNotificationManagerTest)

ReceiveTypingNotificationManagerTest::ReceiveTypingNotificationManagerTest(QObject *parent)
    : QObject(parent)
{
}

void ReceiveTypingNotificationManagerTest::shouldHaveDefaultValues()
{
    const ReceiveTypingNotificationManager manager;
    QVERIFY(manager.typingNotification("Foo"_ba).isEmpty());
}

void ReceiveTypingNotificationManagerTest::shouldAddNotifications()
{
    ReceiveTypingNotificationManager manager;
    QSignalSpy spy(&manager, &ReceiveTypingNotificationManager::notificationChanged);

    manager.insertTypingNotification("room1"_ba, u"user1"_s, false);
    QVERIFY(manager.typingNotification("room1"_ba).isEmpty());
    QCOMPARE(spy.count(), 0);
    spy.clear();

    manager.insertTypingNotification("room1"_ba, u"user1"_s, true);
    QCOMPARE(manager.typingNotification("room1"_ba), u"<strong>user1</strong> is typing..."_s);
    QCOMPARE(spy.count(), 1);
    manager.insertTypingNotification("room1"_ba, u"user1"_s, true);
    QCOMPARE(manager.typingNotification("room1"_ba), u"<strong>user1</strong> is typing..."_s);
    QCOMPARE(spy.count(), 1);

    spy.clear();
    // Add different user.
    manager.insertTypingNotification("room2"_ba, u"user2"_s, true);
    QCOMPARE(manager.typingNotification("room2"_ba), u"<strong>user2</strong> is typing..."_s);
    QCOMPARE(spy.count(), 1);

    manager.insertTypingNotification("room2"_ba, u"user3"_s, true);
    QCOMPARE(manager.typingNotification("room2"_ba), u"<strong>user2 and user3</strong> are typing..."_s);
    QCOMPARE(spy.count(), 2);

    manager.insertTypingNotification("room2"_ba, u"user1"_s, true);
    QCOMPARE(manager.typingNotification("room2"_ba), u"<strong>user2, user3 and user1</strong> are typing..."_s);
    QCOMPARE(spy.count(), 3);

    // Remove typing
    manager.insertTypingNotification("room2"_ba, u"user3"_s, false);
    QCOMPARE(manager.typingNotification("room2"_ba), u"<strong>user2 and user1</strong> are typing..."_s);
    QCOMPARE(spy.count(), 4);

    manager.insertTypingNotification("room2"_ba, u"user1"_s, false);
    QCOMPARE(manager.typingNotification("room2"_ba), u"<strong>user2</strong> is typing..."_s);
    QCOMPARE(spy.count(), 5);

    manager.insertTypingNotification("room2"_ba, u"user3"_s, false);
    QCOMPARE(manager.typingNotification("room2"_ba), u"<strong>user2</strong> is typing..."_s);
    QCOMPARE(spy.count(), 5);

    manager.insertTypingNotification("room2"_ba, u"user2"_s, false);
    QVERIFY(manager.typingNotification("room2"_ba).isEmpty());
    QCOMPARE(spy.count(), 6);

    // No user/roomId
    manager.insertTypingNotification("room2"_ba, u"user2"_s, false);
    QVERIFY(manager.typingNotification("room2"_ba).isEmpty());
    QCOMPARE(spy.count(), 6);

    spy.clear();
    QVERIFY(manager.typingNotification("room5"_ba).isEmpty());
    QCOMPARE(spy.count(), 0);
}

#include "moc_receivetypingnotificationmanagertest.cpp"
