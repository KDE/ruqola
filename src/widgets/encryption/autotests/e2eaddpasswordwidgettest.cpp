/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2eaddpasswordwidgettest.h"
#include "encryption/e2eaddpasswordwidget.h"
#include <QTest>
QTEST_MAIN(E2eAddPasswordWidgetTest)
E2eAddPasswordWidgetTest::E2eAddPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2eAddPasswordWidgetTest::shouldHaveDefaultValues()
{
    E2eAddPasswordWidget w;

    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QCOMPARE(w.position(), KMessageWidget::Header);
    QVERIFY(!w.text().isEmpty());
}

#include "moc_e2eaddpasswordwidgettest.cpp"
