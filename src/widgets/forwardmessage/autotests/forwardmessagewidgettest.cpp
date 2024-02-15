/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "forwardmessagewidgettest.h"
#include "forwardmessage/forwardmessagewidget.h"
#include <QTest>

QTEST_MAIN(ForwardMessageWidgetTest)
ForwardMessageWidgetTest::ForwardMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ForwardMessageWidgetTest::shouldHaveDefaultValues()
{
    ForwardMessageWidget w(nullptr);
    // TODO
}

#include "moc_forwardmessagewidgettest.cpp"
