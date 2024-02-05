/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2eaddpasswordwidgettest.h"
#include "encryption/e2edecodeencryptionkeywidget.h"
#include <QAction>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(E2eAddPasswordWidgetTest)
E2eAddPasswordWidgetTest::E2eAddPasswordWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2eAddPasswordWidgetTest::shouldHaveDefaultValues()
{
    E2eDecodeEncryptionKeyWidget w;
    w.show();
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QCOMPARE(w.position(), KMessageWidget::Header);
    QVERIFY(!w.text().isEmpty());
    QVERIFY(!w.isCloseButtonVisible());

    auto decodeEncryptionKeyAction = w.findChild<QAction *>(QStringLiteral("decodeEncryptionKeyAction"));
    QVERIFY(decodeEncryptionKeyAction);
}

void E2eAddPasswordWidgetTest::shouldEmitSaveSignal()
{
    E2eDecodeEncryptionKeyWidget w;
    QSignalSpy saveSignal(&w, &E2eDecodeEncryptionKeyWidget::decodeEncrytionKey);
    auto decodeEncryptionKeyAction = w.findChild<QAction *>(QStringLiteral("decodeEncryptionKeyAction"));
    decodeEncryptionKeyAction->trigger();
    QCOMPARE(saveSignal.count(), 1);
}

#include "moc_e2eaddpasswordwidgettest.cpp"
