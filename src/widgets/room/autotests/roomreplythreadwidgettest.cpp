/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomreplythreadwidgettest.h"
#include "room/roomreplythreadwidget.h"
#include <QPushButton>
#include <QTest>
QTEST_MAIN(RoomReplyThreadWidgetTest)

RoomReplyThreadWidgetTest::RoomReplyThreadWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void RoomReplyThreadWidgetTest::shouldHaveDefaultValues()
{
    RoomReplyThreadWidget w;

    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QVERIFY(w.wordWrap());
    QVERIFY(!w.isVisible());
}

#include "moc_roomreplythreadwidgettest.cpp"
