/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochanneldialogtest.h"
#include "teams/teamconverttochanneldialog.h"
#include "teams/teamconverttochannelwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamConvertToChannelDialogTest)
TeamConvertToChannelDialogTest::TeamConvertToChannelDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void TeamConvertToChannelDialogTest::shouldHaveDefaultValues()
{
    TeamConvertToChannelDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
    auto mTeamConvertToChannelWidget = d.findChild<TeamConvertToChannelWidget *>(QStringLiteral("mTeamConvertToChannelWidget"));
    QVERIFY(mTeamConvertToChannelWidget);
}
