/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelsdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto mTeamChannelsWidget = d.findChild<TeamChannelsWidget *>(u"mTeamChannelsWidget"_s);
    QVERIFY(mTeamChannelsWidget);
}

#include "moc_teamchannelsdialogtest.cpp"
