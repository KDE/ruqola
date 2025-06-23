/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "accountsoverviewwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/accountsoverviewwidget.h"
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QTabBar>
#include <QTest>
QTEST_MAIN(AccountsOverviewWidgetTest)
AccountsOverviewWidgetTest::AccountsOverviewWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AccountsOverviewWidgetTest::shouldHaveDefaultValues()
{
    AccountsOverviewWidget w;
    auto mTabBar = w.findChild<QTabBar *>(u"mTabBar"_s);
    QVERIFY(mTabBar);
    QCOMPARE(mTabBar->shape(), QTabBar::RoundedSouth);

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_accountsoverviewwidgettest.cpp"
