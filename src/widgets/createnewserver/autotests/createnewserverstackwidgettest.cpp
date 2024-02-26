/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createnewserverstackwidgettest.h"
#include "createnewserver/checknewserverurlwidget.h"
#include "createnewserver/createnewserverstackwidget.h"
#include "createnewserver/createnewserverwidget.h"
#include <QTest>
QTEST_MAIN(CreateNewServerStackWidgetTest)
CreateNewServerStackWidgetTest::CreateNewServerStackWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CreateNewServerStackWidgetTest::shouldHaveDefaultValues()
{
    CreateNewServerStackWidget w;

    auto mCheckNewServerUrlWidget = w.findChild<CheckNewServerUrlWidget *>(QStringLiteral("CheckNewServerUrlWidget"));
    QVERIFY(mCheckNewServerUrlWidget);

    auto mCreateNewServerWidget = w.findChild<CreateNewServerWidget *>(QStringLiteral("mCreateNewServerWidget"));
    QVERIFY(mCreateNewServerWidget);
}

#include "moc_createnewserverstackwidgettest.cpp"
