/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelsdialogtest.h"
#include "teams/teamchannelsdialog.h"
#include "teams/teamchannelswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamChannelsDialogTest)

TeamChannelsDialogTest::TeamChannelsDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void TeamChannelsDialogTest::shouldHaveDefaultValues()
{
    TeamChannelsDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    auto mTeamChannelsWidget = d.findChild<TeamChannelsWidget *>(QStringLiteral("mTeamChannelsWidget"));
    QVERIFY(mTeamChannelsWidget);
}

#include "moc_teamchannelsdialogtest.cpp"
