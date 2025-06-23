/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationrangewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto fromLabel = w.findChild<QLabel *>(u"fromLabel"_s);
    QVERIFY(fromLabel);
    QVERIFY(!fromLabel->text().isEmpty());

    auto toLabel = w.findChild<QLabel *>(u"toLabel"_s);
    QVERIFY(toLabel);
    QVERIFY(!toLabel->text().isEmpty());

    auto mFromDate = w.findChild<QDateEdit *>(u"mFromDate"_s);
    QVERIFY(mFromDate);
    auto mToDate = w.findChild<QDateEdit *>(u"mToDate"_s);
    QVERIFY(mToDate);

    auto mFilterDate = w.findChild<QToolButton *>(u"mFilterDate"_s);
    QVERIFY(mFilterDate);
    QVERIFY(mFilterDate->autoRaise());
}

#include "moc_administratormoderationrangewidgettest.cpp"
