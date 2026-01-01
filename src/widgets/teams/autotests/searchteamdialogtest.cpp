/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchteamdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "teams/searchteamdialog.h"
#include "teams/searchteamwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SearchTeamDialogTest)
SearchTeamDialogTest::SearchTeamDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void SearchTeamDialogTest::shouldHaveDefaultValues()
{
    SearchTeamDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto mSearchTeamWidget = d.findChild<SearchTeamWidget *>(u"mSearchTeamWidget"_s);
    QVERIFY(mSearchTeamWidget);
}

#include "moc_searchteamdialogtest.cpp"
