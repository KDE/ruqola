/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "encryptionutilstest.h"
#include "encryption/encryptionutils.h"
#include <QTest>
QTEST_MAIN(EncryptionUtilsTest)
EncryptionUtilsTest::EncryptionUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void EncryptionUtilsTest::shouldHaveDefaultValues()
{
    // TODO
}

#include "moc_encryptionutilstest.cpp"
