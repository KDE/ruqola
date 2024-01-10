/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

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
    w.show();
    QVERIFY(!w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QVERIFY(w.wordWrap());
}

#include "moc_roomreplythreadwidgettest.cpp"
