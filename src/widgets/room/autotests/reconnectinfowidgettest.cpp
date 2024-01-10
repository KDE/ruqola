/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    w.show();
    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QCOMPARE(w.reconnectSecondDelay(), -1);
}

#include "moc_reconnectinfowidgettest.cpp"
