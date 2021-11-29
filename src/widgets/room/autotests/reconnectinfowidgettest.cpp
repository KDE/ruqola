/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "reconnectinfowidgettest.h"
#include "room/reconnectinfowidget.h"
#include <QTest>
QTEST_MAIN(ReconnectInfoWidgetTest)

ReconnectInfoWidgetTest::ReconnectInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ReconnectInfoWidgetTest::shouldHaveDefaultValues()
{
    ReconnectInfoWidget w;
    QVERIFY(!w.isVisible());
    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QCOMPARE(w.reconnectSecondDelay(), -1);
}
