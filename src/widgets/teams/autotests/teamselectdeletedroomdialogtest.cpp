/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomdialogtest.h"
#include "teams/teamselectdeletedroomdialog.h"
#include "teams/teamselectdeletedroomwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamSelectDeletedRoomDialogTest)

TeamSelectDeletedRoomDialogTest::TeamSelectDeletedRoomDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void TeamSelectDeletedRoomDialogTest::shouldHaveDefaultValues()
{
    TeamSelectDeletedRoomDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
    auto mTeamSelectDeletedRoomWidget = d.findChild<TeamSelectDeletedRoomWidget *>(QStringLiteral("mTeamSelectDeletedRoomWidget"));
    QVERIFY(mTeamSelectDeletedRoomWidget);

    QVERIFY(d.roomsId().isEmpty());
}

#include "moc_teamselectdeletedroomdialogtest.cpp"
