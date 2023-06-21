/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

#include "moc_roomcounterinfowidgettest.cpp"
