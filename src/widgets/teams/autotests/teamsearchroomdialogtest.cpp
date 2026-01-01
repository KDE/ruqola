/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "teamsearchroomdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
    auto mTeamSearchRoomWidget = d.findChild<TeamSearchRoomWidget *>(u"mTeamSearchRoomWidget"_s);
    QVERIFY(mTeamSearchRoomWidget);
}

#include "moc_teamsearchroomdialogtest.cpp"
