/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "offlinewidgettest.h"
#include "offlinewidget/offlinewidget.h"
#include <QTest>

QTEST_MAIN(OffLineWidgetTest)

OffLineWidgetTest::OffLineWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OffLineWidgetTest::shouldHaveDefaultValues()
{
    OffLineWidget w;
    w.show();
    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Warning);
    QVERIFY(!w.text().isEmpty());
}

#include "moc_offlinewidgettest.cpp"
