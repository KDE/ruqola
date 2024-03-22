/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationconsolewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include "administratordialog/moderationconsole/administratormoderationreportedmessageconsoletreewidget.h"
#include "administratordialog/moderationconsole/administratormoderationreporteduserconsoletreewidget.h"
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

    auto mAdministratorModerationReportedMessageConsoleTreeWidget =
        d.findChild<AdministratorModerationReportedMessageConsoleTreeWidget *>(QStringLiteral("mAdministratorModerationReportedMessageConsoleTreeWidget"));
    QVERIFY(mAdministratorModerationReportedMessageConsoleTreeWidget);

    auto mAdministratorModerationReportedUserConsoleTreeWidget =
        d.findChild<AdministratorModerationReportedUserConsoleTreeWidget *>(QStringLiteral("mAdministratorModerationReportedUserConsoleTreeWidget"));
    QVERIFY(mAdministratorModerationReportedUserConsoleTreeWidget);
}

#include "moc_administratormoderationconsolewidgettest.cpp"
