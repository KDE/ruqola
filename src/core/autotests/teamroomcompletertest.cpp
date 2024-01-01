/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "teamroomcompletertest.h"
#include "teams/teamroomcompleter.h"
#include <QTest>
QTEST_GUILESS_MAIN(TeamRoomCompleterTest)

TeamRoomCompleterTest::TeamRoomCompleterTest(QObject *parent)
    : QObject(parent)
{
}

void TeamRoomCompleterTest::shouldHaveDefaultValues()
{
    TeamRoomCompleter t;
    // TODO
}

#include "moc_teamroomcompletertest.cpp"
