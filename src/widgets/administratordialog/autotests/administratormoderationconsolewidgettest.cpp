/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationconsolewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorModerationConsoleWidgetTest)
AdministratorModerationConsoleWidgetTest::AdministratorModerationConsoleWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorModerationConsoleWidgetTest::shouldHaveDefaultValues()
{
    AdministratorModerationConsoleWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_administratormoderationconsolewidgettest.cpp"
