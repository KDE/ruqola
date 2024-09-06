/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoruserstabwidgettest.h"
#include "administratordialog/users/administratoruserstabwidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorUsersTabWidgetTest)
AdministratorUsersTabWidgetTest::AdministratorUsersTabWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorUsersTabWidgetTest::shouldHaveDefaultValues()
{
    AdministratorUsersTabWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_administratoruserstabwidgettest.cpp"
