/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsearchroomwidgettest.h"
#include "teams/teamsearchroomforteamwidget.h"
#include "teams/teamsearchroomwidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamSearchRoomWidgetTest)
TeamSearchRoomWidgetTest::TeamSearchRoomWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void TeamSearchRoomWidgetTest::shouldHaveDefaultValues()
{
    TeamSearchRoomWidget w(nullptr);
    QVERIFY(w.roomIds().isEmpty());

    auto mTeamSearchRoomForTeamWidget = w.findChild<TeamSearchRoomForTeamWidget *>(QStringLiteral("mTeamSearchRoomForTeamWidget"));
    QVERIFY(mTeamSearchRoomForTeamWidget);

    auto vboxLayout = w.findChild<QVBoxLayout *>(QStringLiteral("vboxLayout"));
    QVERIFY(vboxLayout);
    QCOMPARE(vboxLayout->contentsMargins(), QMargins{});
}

#include "moc_teamsearchroomwidgettest.cpp"
