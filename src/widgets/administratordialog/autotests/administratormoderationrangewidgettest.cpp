/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationrangewidgettest.h"
#include "administratordialog/moderationconsole/administratormoderationrangewidget.h"
#include <QDateEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
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
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

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
}

#include "moc_administratormoderationrangewidgettest.cpp"
