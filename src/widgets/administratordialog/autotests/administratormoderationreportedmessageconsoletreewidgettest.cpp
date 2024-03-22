/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationreportedmessageconsoletreewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include "administratordialog/moderationconsole/administratormoderationreportedmessageconsoletreewidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorModerationReportedMessageConsoleTreeWidgetTest)
AdministratorModerationReportedMessageConsoleTreeWidgetTest::AdministratorModerationReportedMessageConsoleTreeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorModerationReportedMessageConsoleTreeWidgetTest::shouldHaveDefaultValues()
{
    AdministratorModerationConsoleWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mAdministratorModerationReportedMessageConsoleTreeWidget =
        d.findChild<AdministratorModerationReportedMessageConsoleTreeWidget *>(QStringLiteral("mAdministratorModerationReportedMessageConsoleTreeWidget"));
    QVERIFY(mAdministratorModerationReportedMessageConsoleTreeWidget);
}

#include "moc_administratormoderationreportedmessageconsoletreewidgettest.cpp"
