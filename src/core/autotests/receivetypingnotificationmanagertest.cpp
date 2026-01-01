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
    ReceiveTypingNotificationManager manager;
    QVERIFY(manager.typingNotification(QByteArrayLiteral("Foo")).isEmpty());
}

void ReceiveTypingNotificationManagerTest::shouldAddNotifications()
{
    ReceiveTypingNotificationManager manager;
    QSignalSpy spy(&manager, &ReceiveTypingNotificationManager::notificationChanged);

    manager.insertTypingNotification(QByteArrayLiteral("room1"), u"user1"_s, false);
    QVERIFY(manager.typingNotification(QByteArrayLiteral("room1")).isEmpty());
    QCOMPARE(spy.count(), 0);
    spy.clear();

    manager.insertTypingNotification(QByteArrayLiteral("room1"), u"user1"_s, true);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room1")), u"<strong>user1</strong> is typing..."_s);
    QCOMPARE(spy.count(), 1);
    manager.insertTypingNotification(QByteArrayLiteral("room1"), u"user1"_s, true);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room1")), u"<strong>user1</strong> is typing..."_s);
    QCOMPARE(spy.count(), 1);

    spy.clear();
    // Add different user.
    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user2"_s, true);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room2")), u"<strong>user2</strong> is typing..."_s);
    QCOMPARE(spy.count(), 1);

    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user3"_s, true);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room2")), u"<strong>user2 and user3</strong> are typing..."_s);
    QCOMPARE(spy.count(), 2);

    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user1"_s, true);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room2")), u"<strong>user2, user3 and user1</strong> are typing..."_s);
    QCOMPARE(spy.count(), 3);

    // Remove typing
    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user3"_s, false);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room2")), u"<strong>user2 and user1</strong> are typing..."_s);
    QCOMPARE(spy.count(), 4);

    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user1"_s, false);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room2")), u"<strong>user2</strong> is typing..."_s);
    QCOMPARE(spy.count(), 5);

    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user3"_s, false);
    QCOMPARE(manager.typingNotification(QByteArrayLiteral("room2")), u"<strong>user2</strong> is typing..."_s);
    QCOMPARE(spy.count(), 5);

    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user2"_s, false);
    QVERIFY(manager.typingNotification(QByteArrayLiteral("room2")).isEmpty());
    QCOMPARE(spy.count(), 6);

    // No user/roomId
    manager.insertTypingNotification(QByteArrayLiteral("room2"), u"user2"_s, false);
    QVERIFY(manager.typingNotification(QByteArrayLiteral("room2")).isEmpty());
    QCOMPARE(spy.count(), 6);

    spy.clear();
    QVERIFY(manager.typingNotification(QByteArrayLiteral("room5")).isEmpty());
    QCOMPARE(spy.count(), 0);
}

#include "moc_receivetypingnotificationmanagertest.cpp"
