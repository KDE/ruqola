/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeywidgettest.h"
#include "encryption/e2epassworddecodekeywidget.h"
#include <QTest>
QTEST_MAIN(E2ePasswordDecodeKeyWidgetTest)

E2ePasswordDecodeKeyWidgetTest::E2ePasswordDecodeKeyWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2ePasswordDecodeKeyWidgetTest::shouldHaveDefaultValues()
{
    E2ePasswordDecodeKeyWidget w;
    // TODO
}

#include "moc_e2epassworddecodekeywidgettest.cpp"
