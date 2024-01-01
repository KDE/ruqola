/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roominfotest.h"
#include "roominfo/roominfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(RoomInfoTest)

RoomInfoTest::RoomInfoTest(QObject *parent)
    : QObject(parent)
{
}

void RoomInfoTest::shouldHaveDefaultValues()
{
    RoomInfo w;
    QVERIFY(!w.defaultRoom());
    QCOMPARE(w.usersCount(), -1);
    QCOMPARE(w.messageCount(), -1);
    QVERIFY(w.channelType().isEmpty());
    QVERIFY(w.topic().isEmpty());
    QVERIFY(w.announcement().isEmpty());
    QVERIFY(w.description().isEmpty());
    QVERIFY(w.identifier().isEmpty());
    QVERIFY(!w.readOnly());
    QVERIFY(w.name().isEmpty());
    QCOMPARE(w.lastMessage(), -1);
    QVERIFY(!w.featured());
    QVERIFY(w.ownerName().isEmpty());
    QVERIFY(w.belongsTo().isEmpty());
}

#include "moc_roominfotest.cpp"
