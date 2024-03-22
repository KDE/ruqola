/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationreportedmessageconsoletreewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include "administratordialog/moderationconsole/administratormoderationrangewidget.h"
#include "administratordialog/moderationconsole/moderationreportedmessageconsoletreewidget.h"
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

    auto mModerationConsoleTreeWidget = d.findChild<ModerationReportedMessageConsoleTreeWidget *>(QStringLiteral("mModerationConsoleTreeWidget"));
    QVERIFY(mModerationConsoleTreeWidget);

    auto mAdministratorModerationRangeWidget = d.findChild<AdministratorModerationRangeWidget *>(QStringLiteral("mAdministratorModerationRangeWidget"));
    QVERIFY(mAdministratorModerationRangeWidget);
}

#include "moc_administratormoderationreportedmessageconsoletreewidgettest.cpp"
