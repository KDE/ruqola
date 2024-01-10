/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomquotemessagewidgettest.h"
#include "room/roomquotemessagewidget.h"
#include <QTest>
QTEST_MAIN(RoomQuoteMessageWidgetTest)
RoomQuoteMessageWidgetTest::RoomQuoteMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomQuoteMessageWidgetTest::shouldHaveDefaultValues()
{
    RoomQuoteMessageWidget w;
    w.show();
    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QVERIFY(w.wordWrap());
}

#include "moc_roomquotemessagewidgettest.cpp"
