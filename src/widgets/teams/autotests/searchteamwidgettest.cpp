/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchteamwidgettest.h"
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

    auto mainLayout = d.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto label = d.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mSearchLine = d.findChild<QLineEdit *>(QStringLiteral("mSearchLine"));
    QVERIFY(mSearchLine);
    QVERIFY(mSearchLine->text().isEmpty());
}
