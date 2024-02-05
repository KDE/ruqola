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
    w.show();
    QCOMPARE(w.messageType(), KMessageWidget::Error);
    QCOMPARE(w.position(), KMessageWidget::Header);
    QVERIFY(!w.text().isEmpty());
    QVERIFY(w.isCloseButtonVisible());
}

#include "moc_e2edecodeencryptionkeyfailedwidgettest.cpp"
