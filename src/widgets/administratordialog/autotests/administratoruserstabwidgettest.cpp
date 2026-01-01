/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoruserstabwidgettest.h"
#include "administratordialog/users/administratoruserstabwidget.h"
#include "administratordialog/users/administratoruserswidget.h"
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(AdministratorUsersTabWidgetTest)
AdministratorUsersTabWidgetTest::AdministratorUsersTabWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorUsersTabWidgetTest::shouldHaveDefaultValues()
{
    AdministratorUsersTabWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAllUsers = w.findChild<AdministratorUsersWidget *>("mAllUsers"_L1);
    QVERIFY(mAllUsers);

    auto mPendingUsers = w.findChild<AdministratorUsersWidget *>("mPendingUsers"_L1);
    QVERIFY(mPendingUsers);

    auto mDeactivatedUsers = w.findChild<AdministratorUsersWidget *>("mDeactivatedUsers"_L1);
    QVERIFY(mDeactivatedUsers);

    auto mActivatedUsers = w.findChild<AdministratorUsersWidget *>("mActivatedUsers"_L1);
    QVERIFY(mActivatedUsers);

    auto mTabWidget = w.findChild<QTabWidget *>("mTabWidget"_L1);
    QVERIFY(mTabWidget);
}

#include "moc_administratoruserstabwidgettest.cpp"
