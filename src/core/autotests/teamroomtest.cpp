/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamroomtest.h"
#include "teams/teamroom.h"
#include <QTest>
QTEST_GUILESS_MAIN(TeamRoomTest)
TeamRoomTest::TeamRoomTest(QObject *parent)
    : QObject(parent)
{
}

void TeamRoomTest::shouldHaveDefaultValue()
{
    TeamRoom w;
    QVERIFY(!w.autoJoin());
    QVERIFY(w.name().isEmpty());
    QVERIFY(w.fname().isEmpty());
}

#include "moc_teamroomtest.cpp"
