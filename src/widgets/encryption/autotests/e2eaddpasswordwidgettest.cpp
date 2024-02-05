/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2eaddpasswordwidgettest.h"
#include "encryption/e2eaddpasswordwidget.h"
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
    E2eAddPasswordWidget w;
    w.show();
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QCOMPARE(w.position(), KMessageWidget::Header);
    QVERIFY(!w.text().isEmpty());
    QVERIFY(!w.isCloseButtonVisible());

    auto savePasswordAction = w.findChild<QAction *>(QStringLiteral("refreshKeysAction"));
    QVERIFY(savePasswordAction);
}

void E2eAddPasswordWidgetTest::shouldEmitSaveSignal()
{
    E2eAddPasswordWidget w;
    QSignalSpy saveSignal(&w, &E2eAddPasswordWidget::decodeEncrytionKey);
    auto savePasswordAction = w.findChild<QAction *>(QStringLiteral("refreshKeysAction"));
    savePasswordAction->trigger();
    QCOMPARE(saveSignal.count(), 1);
}

#include "moc_e2eaddpasswordwidgettest.cpp"
