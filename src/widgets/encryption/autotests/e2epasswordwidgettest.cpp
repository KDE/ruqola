/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epasswordwidgettest.h"
#include "encryption/e2epasswordwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>
#include <QToolButton>
#include <QVBoxLayout>
QTEST_MAIN(E2ePasswordWidgetTest)
using namespace Qt::Literals::StringLiterals;
E2ePasswordWidgetTest::E2ePasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2ePasswordWidgetTest::shouldHaveDefaultValues()
{
    E2ePasswordWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>("mainLayout"_L1);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
}

#include "moc_e2epasswordwidgettest.cpp"
