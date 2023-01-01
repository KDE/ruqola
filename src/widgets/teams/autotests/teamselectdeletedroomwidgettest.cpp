/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomwidgettest.h"
#include "teams/teamselectdeletedroomwidget.h"
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TeamSelectDeletedRoomWidgetTest)
TeamSelectDeletedRoomWidgetTest::TeamSelectDeletedRoomWidgetTest(QWidget *parent)
    : QWidget(parent)
{
}

void TeamSelectDeletedRoomWidgetTest::shouldHaveDefaultValues()
{
    TeamSelectDeletedRoomWidget w;

    QVERIFY(w.roomsId().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<QListView *>(QStringLiteral("mListView"));
    QVERIFY(mListView);

    auto mSearchLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
}
