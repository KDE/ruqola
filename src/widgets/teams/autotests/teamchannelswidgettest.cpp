/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelswidgettest.h"
#include "teams/teamchannelscombobox.h"
#include "teams/teamchannelswidget.h"
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamChannelsWidgetTest)
TeamChannelsWidgetTest::TeamChannelsWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void TeamChannelsWidgetTest::shouldHaveDefaultValues()
{
    TeamChannelsWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<QListView *>(QStringLiteral("mListView"));
    QVERIFY(mListView);
    auto mSearchLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mTeamChannelsCombobox = w.findChild<TeamChannelsComboBox *>(QStringLiteral("mTeamChannelsCombobox"));
    QVERIFY(mTeamChannelsCombobox);
}
