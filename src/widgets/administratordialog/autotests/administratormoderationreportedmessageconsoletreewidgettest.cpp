/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationreportedmessageconsoletreewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->spacing(), 0);

    auto mModerationConsoleTreeWidget = d.findChild<ModerationReportedMessageConsoleTreeWidget *>(u"mModerationConsoleTreeWidget"_s);
    QVERIFY(mModerationConsoleTreeWidget);
    auto mAdministratorModerationRangeWidget = d.findChild<AdministratorModerationRangeWidget *>(u"mAdministratorModerationRangeWidget"_s);
    QVERIFY(mAdministratorModerationRangeWidget);
}

#include "moc_administratormoderationreportedmessageconsoletreewidgettest.cpp"
