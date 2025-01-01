/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationreportedmessageconsoletreewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationreportedmessageconsoletreewidget.h"
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
    AdministratorModerationReportedMessageConsoleTreeWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->spacing(), 0);

    auto mModerationConsoleTreeWidget = d.findChild<ModerationReportedMessageConsoleTreeWidget *>(QStringLiteral("mModerationConsoleTreeWidget"));
    QVERIFY(mModerationConsoleTreeWidget);
    auto mAdministratorModerationRangeWidget = d.findChild<AdministratorModerationRangeWidget *>(QStringLiteral("mAdministratorModerationRangeWidget"));
    QVERIFY(mAdministratorModerationRangeWidget);
}

#include "moc_administratormoderationreportedmessageconsoletreewidgettest.cpp"
