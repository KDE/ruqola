/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchteamdialogtest.h"
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

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    auto mSearchTeamWidget = d.findChild<SearchTeamWidget *>(QStringLiteral("mSearchTeamWidget"));
    QVERIFY(mSearchTeamWidget);
}
