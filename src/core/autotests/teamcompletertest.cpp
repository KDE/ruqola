/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamcompletertest.h"
#include "teams/teamcompleter.h"
#include <QTest>

QTEST_GUILESS_MAIN(TeamCompleterTest)
TeamCompleterTest::TeamCompleterTest(QObject *parent)
    : QObject(parent)
{
}

void TeamCompleterTest::shouldHaveDefaultValues()
{
    TeamCompleter t;
    // TODO
}
