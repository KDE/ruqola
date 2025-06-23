/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2esaveencryptionkeywidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "encryption/e2esaveencryptionkeywidget.h"
#include <QAction>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(E2eSaveEncryptionKeyWidgetTest)
E2eSaveEncryptionKeyWidgetTest::E2eSaveEncryptionKeyWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void E2eSaveEncryptionKeyWidgetTest::shouldHaveDefaultValues()
{
    E2eSaveEncryptionKeyWidget w;
    w.show();
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QCOMPARE(w.position(), KMessageWidget::Header);
    QVERIFY(!w.text().isEmpty());
    QVERIFY(!w.isCloseButtonVisible());

    auto decodeEncryptionKeyAction = w.findChild<QAction *>(u"decodeEncryptionKeyAction"_s);
    QVERIFY(decodeEncryptionKeyAction);
}

void E2eSaveEncryptionKeyWidgetTest::shouldEmitSaveSignal()
{
    E2eSaveEncryptionKeyWidget w;
    QSignalSpy saveSignal(&w, &E2eSaveEncryptionKeyWidget::saveEncrytionKey);
    auto decodeEncryptionKeyAction = w.findChild<QAction *>(u"decodeEncryptionKeyAction"_s);
    decodeEncryptionKeyAction->trigger();
    QCOMPARE(saveSignal.count(), 1);
}

#include "moc_e2esaveencryptionkeywidgettest.cpp"
