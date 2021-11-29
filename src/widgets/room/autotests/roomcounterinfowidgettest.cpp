/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "roomcounterinfowidgettest.h"
#include "room/roomcounterinfowidget.h"
#include <QTest>
QTEST_MAIN(RoomCounterInfoWidgetTest)
RoomCounterInfoWidgetTest::RoomCounterInfoWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomCounterInfoWidgetTest::shouldHaveDefaultValues()
{
    RoomCounterInfoWidget w;
    QVERIFY(!w.channelCounterInfo().isValid());
    QVERIFY(!w.isVisible());
    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
}
