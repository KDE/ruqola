/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "typingnotificationtest.h"
#include "libruqola_private_export.h"
#include "typingnotification.h"
#include <QSignalSpy>

#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(TypingNotificationTest)

extern LIBRUQOLACORE_TESTS_EXPORT int timerTimeOutValueMs;

TypingNotificationTest::TypingNotificationTest(QObject *parent)
    : QObject(parent)
{
}

void TypingNotificationTest::shouldNotEmitSignalByDefault()
{
    timerTimeOutValueMs = 100; // 100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    // Wait 500ms
    QTest::qSleep(200);
    QCOMPARE(signal.count(), 0);
}

void TypingNotificationTest::shouldEmitSignalWhenTyping()
{
    timerTimeOutValueMs = 100; // 100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    t.textNotificationChanged("foo"_ba, false);
    // Wait 500ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);
}

void TypingNotificationTest::shouldEmitSignalWhenTypingAndEmitTypingFalseAfterTimeout()
{
    timerTimeOutValueMs = 100; // 100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    t.textNotificationChanged("foo"_ba, false);
    // Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);

    QTest::qWait(150); // 150 ms
    QCOMPARE(signal.count(), 2);
}

void TypingNotificationTest::shouldDontEmitSignalWhenTypingSeveralTextBeforeTimeOut()
{
    timerTimeOutValueMs = 100; // 100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    t.textNotificationChanged("foo"_ba, false);
    // Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);
    t.textNotificationChanged("foo"_ba, false);
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);
    t.textNotificationChanged("foo"_ba, false);
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);

    QTest::qWait(150); // 150 ms
    QCOMPARE(signal.count(), 2);
}

void TypingNotificationTest::shouldEmitTwoSignalWhenChangeRoom()
{
    timerTimeOutValueMs = 100; // 100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    QByteArray room1 = "room1"_ba;
    QByteArray room2 = "room2"_ba;
    t.textNotificationChanged(room1, false);
    // Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);

    t.textNotificationChanged(room2, false);
    // Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 3); // Two signal send.
}

#include "moc_typingnotificationtest.cpp"
