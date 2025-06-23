/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "createnewserverstackwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mCheckNewServerUrlWidget = w.findChild<CheckNewServerUrlWidget *>(u"mCheckNewServerUrlWidget"_s);
    QVERIFY(mCheckNewServerUrlWidget);

    auto mCreateNewServerWidget = w.findChild<CreateNewServerWidget *>(u"mCreateNewServerWidget"_s);
    QVERIFY(mCreateNewServerWidget);
}

#include "moc_createnewserverstackwidgettest.cpp"
