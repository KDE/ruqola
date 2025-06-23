/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamconverttochanneldialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
    auto mTeamConvertToChannelWidget = d.findChild<TeamConvertToChannelWidget *>(u"mTeamConvertToChannelWidget"_s);
    QVERIFY(mTeamConvertToChannelWidget);
}

#include "moc_teamconverttochanneldialogtest.cpp"
