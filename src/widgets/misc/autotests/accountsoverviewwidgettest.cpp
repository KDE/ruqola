/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsoverviewwidgettest.h"
#include "misc/accountsoverviewwidget.h"
#include <QHBoxLayout>
#include <QTabBar>
#include <QTest>
QTEST_MAIN(AccountsOverviewWidgetTest)
AccountsOverviewWidgetTest::AccountsOverviewWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AccountsOverviewWidgetTest::shouldHaveDefaultValues()
{
    AccountsOverviewWidget w;
    auto mTabBar = w.findChild<QTabBar *>(QStringLiteral("mTabBar"));
    QVERIFY(mTabBar);
    QCOMPARE(mTabBar->shape(), QTabBar::RoundedSouth);

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}
