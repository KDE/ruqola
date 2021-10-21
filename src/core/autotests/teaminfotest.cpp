/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
