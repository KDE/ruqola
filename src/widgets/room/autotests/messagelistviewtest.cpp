/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistviewtest.h"
#include "room/messagelistview.h"
#include <QTest>
QTEST_MAIN(MessageListViewTest)

MessageListViewTest::MessageListViewTest(QObject *parent)
    : QObject{parent}
{
}

void MessageListViewTest::shouldHaveDefaultValues()
{
    MessageListView w(nullptr, MessageListView::Mode::Editing);
    QCOMPARE(w.mode(), MessageListView::Mode::Editing);
    QCOMPARE(w.selectionMode(), QAbstractItemView::NoSelection);
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(w.verticalScrollMode(), QAbstractItemView::ScrollPerPixel);
    QVERIFY(w.wordWrap());
    QCOMPARE(w.focusPolicy(), Qt::NoFocus);
}

#include "moc_messagelistviewtest.cpp"
