/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratormoderationconsolewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationconsolewidget.h"
#include "administratordialog/moderationconsole/administratormoderationrangewidget.h"
#include "administratordialog/moderationconsole/moderationconsoletreewidget.h"
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

    auto mModerationConsoleTreeWidget = d.findChild<ModerationConsoleTreeWidget *>(QStringLiteral("mModerationConsoleTreeWidget"));
    QVERIFY(mModerationConsoleTreeWidget);

    auto mAdministratorModerationRangeWidget = d.findChild<AdministratorModerationRangeWidget *>(QStringLiteral("mAdministratorModerationRangeWidget"));
    QVERIFY(mAdministratorModerationRangeWidget);
}

#include "moc_administratormoderationconsolewidgettest.cpp"
