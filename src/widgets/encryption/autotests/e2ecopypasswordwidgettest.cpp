/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypasswordwidgettest.h"
#include "encryption/e2ecopypasswordwidget.h"
#include <QTest>
QTEST_MAIN(E2eCopyPasswordWidgetTest)
E2eCopyPasswordWidgetTest::E2eCopyPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2eCopyPasswordWidgetTest::shouldHaveDefaultValues()
{
    E2eCopyPasswordWidget w;
    // TODO
}
