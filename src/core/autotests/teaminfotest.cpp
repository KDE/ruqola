/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "teaminfotest.h"
#include "teams/teaminfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(TeamInfoTest)
TeamInfoTest::TeamInfoTest(QObject *parent)
    : QObject(parent)
{
}

void TeamInfoTest::shouldHaveDefaultValues()
{
    TeamInfo info;
    QVERIFY(info.teamId().isEmpty());
    QVERIFY(!info.mainTeam());
    QVERIFY(!info.autoJoin());
    QVERIFY(!info.isValid());
    QVERIFY(!info.hasTeamRoom());
    QCOMPARE(info.roomsCount(), 0);
}

void TeamInfoTest::shouldVerifyHasTeamRoom()
{
    TeamInfo info;
    QVERIFY(!info.hasTeamRoom());
    info.setMainTeam(true);
    info.setTeamId(QStringLiteral("ff"));
    QVERIFY(!info.hasTeamRoom());
    info.setMainTeam(false);
    QVERIFY(info.hasTeamRoom());
    info.setTeamId(QString());
    QVERIFY(!info.hasTeamRoom());
}
