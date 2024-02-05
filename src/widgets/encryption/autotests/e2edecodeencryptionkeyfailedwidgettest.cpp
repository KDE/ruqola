/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edecodeencryptionkeyfailedwidgettest.h"
#include "encryption/e2edecodeencryptionkeyfailedwidget.h"
#include <QTest>
QTEST_MAIN(E2eDecodeEncryptionKeyFailedWidgetTest)
E2eDecodeEncryptionKeyFailedWidgetTest::E2eDecodeEncryptionKeyFailedWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2eDecodeEncryptionKeyFailedWidgetTest::shouldHaveDefaultValues()
{
    E2eDecodeEncryptionKeyFailedWidget w;
    // TODO
}
