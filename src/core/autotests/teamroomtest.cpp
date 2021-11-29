/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
