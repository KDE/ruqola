/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisabledialogtest.h"
#include <QTest>

QTEST_MAIN(E2eDisableDialogTest)

E2eDisableDialogTest::E2eDisableDialogTest(QObject *parent)
    : QObject{parent}
{
}

E2eDisableDialogTest::~E2eDisableDialogTest() = default;

#include "moc_e2edisabledialogtest.cpp"
