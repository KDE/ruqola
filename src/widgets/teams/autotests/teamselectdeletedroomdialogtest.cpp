/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
    auto mTeamSelectDeletedRoomWidget = d.findChild<TeamSelectDeletedRoomWidget *>(u"mTeamSelectDeletedRoomWidget"_s);
    QVERIFY(mTeamSelectDeletedRoomWidget);

    QVERIFY(d.roomsId().isEmpty());
}

#include "moc_teamselectdeletedroomdialogtest.cpp"
