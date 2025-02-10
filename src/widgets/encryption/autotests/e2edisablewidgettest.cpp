/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisablewidgettest.h"
#include <QTest>

QTEST_MAIN(E2eDisableWidgetTest)

E2eDisableWidgetTest::E2eDisableWidgetTest(QObject *parent)
    : QObject{parent}
{
}

E2eDisableWidgetTest::~E2eDisableWidgetTest() = default;

#include "moc_e2edisablewidgettest.cpp"
