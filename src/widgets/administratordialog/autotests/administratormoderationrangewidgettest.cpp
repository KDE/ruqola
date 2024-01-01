/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationrangewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationrangewidget.h"
#include <QDateEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
#include <QToolButton>
QTEST_MAIN(AdministratorModerationRangeWidgetTest)
AdministratorModerationRangeWidgetTest::AdministratorModerationRangeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorModerationRangeWidgetTest::shouldHaveDefaultValues()
{
    AdministratorModerationRangeWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto fromLabel = w.findChild<QLabel *>(QStringLiteral("fromLabel"));
    QVERIFY(fromLabel);
    QVERIFY(!fromLabel->text().isEmpty());

    auto toLabel = w.findChild<QLabel *>(QStringLiteral("toLabel"));
    QVERIFY(toLabel);
    QVERIFY(!toLabel->text().isEmpty());

    auto mFromDate = w.findChild<QDateEdit *>(QStringLiteral("mFromDate"));
    QVERIFY(mFromDate);
    auto mToDate = w.findChild<QDateEdit *>(QStringLiteral("mToDate"));
    QVERIFY(mToDate);

    auto mFilterDate = w.findChild<QToolButton *>(QStringLiteral("mFilterDate"));
    QVERIFY(mFilterDate);
    QVERIFY(mFilterDate->autoRaise());
}

#include "moc_administratormoderationrangewidgettest.cpp"
