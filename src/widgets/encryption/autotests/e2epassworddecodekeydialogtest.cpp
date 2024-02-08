/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2epassworddecodekeydialogtest.h"
#include "encryption/e2epassworddecodekeydialog.h"
#include <QTest>
QTEST_MAIN(E2ePasswordDecodeKeyDialogTest)
E2ePasswordDecodeKeyDialogTest::E2ePasswordDecodeKeyDialogTest(QObject *parent)
    : QObject{parent}
{
}

void E2ePasswordDecodeKeyDialogTest::shouldHaveDefaultValues()
{
    E2ePasswordDecodeKeyDialog w;
    // TODO
}

#include "moc_e2epassworddecodekeydialogtest.cpp"
