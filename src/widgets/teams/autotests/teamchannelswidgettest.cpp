/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamchannelswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto hboxLayout = w.findChild<QHBoxLayout *>(u"hboxLayout"_s);
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<QListView *>(u"mListView"_s);
    QVERIFY(mListView);
    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mTeamChannelsCombobox = w.findChild<TeamChannelsComboBox *>(u"mTeamChannelsCombobox"_s);
    QVERIFY(mTeamChannelsCombobox);
}

#include "moc_teamchannelswidgettest.cpp"
