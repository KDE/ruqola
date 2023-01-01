/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "teamsearchroomdialogtest.h"
#include "teams/teamsearchroomdialog.h"
#include "teams/teamsearchroomwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamSearchRoomDialogTest)

TeamSearchRoomDialogTest::TeamSearchRoomDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void TeamSearchRoomDialogTest::shouldHaveDefaultValues()
{
    TeamSearchRoomDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
    auto mTeamSearchRoomWidget = d.findChild<TeamSearchRoomWidget *>(QStringLiteral("mTeamSearchRoomWidget"));
    QVERIFY(mTeamSearchRoomWidget);
}
