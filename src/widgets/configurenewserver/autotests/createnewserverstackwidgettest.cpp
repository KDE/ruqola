/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createnewserverstackwidgettest.h"
#include "configurenewserver/checknewserverurlwidget.h"
#include "configurenewserver/createnewserverstackwidget.h"
#include "configurenewserver/createnewserverwidget.h"
#include <QTest>
QTEST_MAIN(CreateNewServerStackWidgetTest)
CreateNewServerStackWidgetTest::CreateNewServerStackWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CreateNewServerStackWidgetTest::shouldHaveDefaultValues()
{
    CreateNewServerStackWidget w;

    auto mCheckNewServerUrlWidget = w.findChild<CheckNewServerUrlWidget *>(QStringLiteral("mCheckNewServerUrlWidget"));
    QVERIFY(mCheckNewServerUrlWidget);

    auto mCreateNewServerWidget = w.findChild<CreateNewServerWidget *>(QStringLiteral("mCreateNewServerWidget"));
    QVERIFY(mCreateNewServerWidget);
}

#include "moc_createnewserverstackwidgettest.cpp"
