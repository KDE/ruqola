/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2ecopypassworddialogtest.h"
#include "encryption/e2ecopypassworddialog.h"
#include <QTest>
QTEST_MAIN(E2eCopyPasswordDialogTest)
E2eCopyPasswordDialogTest::E2eCopyPasswordDialogTest(QObject *parent)
    : QObject{parent}
{
}

void E2eCopyPasswordDialogTest::shouldHaveDefaultValues()
{
    E2eCopyPasswordDialog d;
    // TODO
}

#include "moc_e2ecopypassworddialogtest.cpp"
