/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationconsolewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mAdministratorModerationReportedMessageConsoleTreeWidget =
        d.findChild<AdministratorModerationReportedMessageConsoleTreeWidget *>(u"mAdministratorModerationReportedMessageConsoleTreeWidget"_s);
    QVERIFY(mAdministratorModerationReportedMessageConsoleTreeWidget);

    auto mAdministratorModerationReportedUserConsoleTreeWidget =
        d.findChild<AdministratorModerationReportedUserConsoleTreeWidget *>(u"mAdministratorModerationReportedUserConsoleTreeWidget"_s);
    QVERIFY(mAdministratorModerationReportedUserConsoleTreeWidget);
}

#include "moc_administratormoderationconsolewidgettest.cpp"
