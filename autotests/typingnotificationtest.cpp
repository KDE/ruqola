/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "typingnotificationtest.h"
#include "libruqola_private_export.h"
#include "typingnotification.h"
#include <QSignalSpy>

#include <QTest>
QTEST_GUILESS_MAIN(TypingNotificationTest)

extern LIBRUQOLACORE_TESTS_EXPORT int timerTimeOutValueMs;

TypingNotificationTest::TypingNotificationTest(QObject *parent)
    : QObject(parent)
{
}

void TypingNotificationTest::shouldNotEmitSignalByDefault()
{
    timerTimeOutValueMs = 100; //100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    //Wait 500ms
    QTest::qSleep(200);
    QCOMPARE(signal.count(), 0);
}

void TypingNotificationTest::shouldEmitSignalWhenTyping()
{
    timerTimeOutValueMs = 100; //100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    t.setText(QStringLiteral("foo"), QStringLiteral("bla"));
    //Wait 500ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);
}

void TypingNotificationTest::shouldEmitSignalWhenTypingAndEmitTypingFalseAfterTimeout()
{
    timerTimeOutValueMs = 100; //100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    t.setText(QStringLiteral("foo"), QStringLiteral("bla"));
    //Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);

    QTest::qWait(150); // 150 ms
    QCOMPARE(signal.count(), 2);
}

void TypingNotificationTest::shouldDontEmitSignalWhenTypingSeveralTextBeforeTimeOut()
{
    timerTimeOutValueMs = 100; //100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    t.setText(QStringLiteral("foo"), QStringLiteral("bla"));
    //Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);
    t.setText(QStringLiteral("foo"), QStringLiteral("bla1"));
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);
    t.setText(QStringLiteral("foo"), QStringLiteral("bla2"));
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);

    QTest::qWait(150); // 150 ms
    QCOMPARE(signal.count(), 2);
}

void TypingNotificationTest::shouldEmitTwoSignalWhenChangeRoom()
{
    timerTimeOutValueMs = 100; //100ms
    TypingNotification t;
    QSignalSpy signal(&t, &TypingNotification::informTypingStatus);
    QString room1 = QStringLiteral("room1");
    QString room2 = QStringLiteral("room2");
    t.setText(room1, QStringLiteral("bla"));
    //Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 1);

    t.setText(room2, QStringLiteral("bla"));
    //Wait 50ms
    QTest::qWait(50);
    QCOMPARE(signal.count(), 3); //Two signal send.
}
