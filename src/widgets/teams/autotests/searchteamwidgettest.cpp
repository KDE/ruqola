/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchteamwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "teams/searchteamwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(SearchTeamWidgetTest)

SearchTeamWidgetTest::SearchTeamWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SearchTeamWidgetTest::shouldHaveDefaultValues()
{
    SearchTeamWidget d(nullptr);

    auto mainLayout = d.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto label = d.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mSearchLine = d.findChild<QLineEdit *>(u"mSearchLine"_s);
    QVERIFY(mSearchLine);
    QVERIFY(mSearchLine->text().isEmpty());
}

#include "moc_searchteamwidgettest.cpp"
