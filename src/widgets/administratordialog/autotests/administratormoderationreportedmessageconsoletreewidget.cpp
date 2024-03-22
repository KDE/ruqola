/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationreportedmessageconsoletreewidget.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include "administratordialog/moderationconsole/administratormoderationrangewidget.h"
#include "administratordialog/moderationconsole/moderationreportedmessageconsoletreewidget.h"
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorModerationReportedMessageConsoleTreeWidget)
AdministratorModerationReportedMessageConsoleTreeWidget::AdministratorModerationReportedMessageConsoleTreeWidget(QObject *parent)
    : QObject{parent}
{
}

void AdministratorModerationReportedMessageConsoleTreeWidget::shouldHaveDefaultValues()
{
    AdministratorModerationConsoleWidget d(nullptr);
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mModerationConsoleTreeWidget = d.findChild<ModerationReportedMessageConsoleTreeWidget *>(QStringLiteral("mModerationConsoleTreeWidget"));
    QVERIFY(mModerationConsoleTreeWidget);

    auto mAdministratorModerationRangeWidget = d.findChild<AdministratorModerationRangeWidget *>(QStringLiteral("mAdministratorModerationRangeWidget"));
    QVERIFY(mAdministratorModerationRangeWidget);
}

#include "moc_administratormoderationreportedmessageconsoletreewidget.cpp"
