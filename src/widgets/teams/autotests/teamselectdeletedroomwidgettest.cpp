/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamselectdeletedroomwidgettest.h"

#include "teams/teamselectdeletedroomwidget.h"
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(TeamSelectDeletedRoomWidgetTest)
TeamSelectDeletedRoomWidgetTest::TeamSelectDeletedRoomWidgetTest(QWidget *parent)
    : QWidget(parent)
{
}

void TeamSelectDeletedRoomWidgetTest::shouldHaveDefaultValues()
{
    const TeamSelectDeletedRoomWidget w;

    QVERIFY(w.roomsId().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<QListView *>(u"mListView"_s);
    QVERIFY(mListView);

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
}

#include "moc_teamselectdeletedroomwidgettest.cpp"
