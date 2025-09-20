/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomtreeviewtest.h"
#include "room/usersinroomtreeview.h"
#include <QTest>
QTEST_MAIN(UsersInRoomTreeViewTest)

UsersInRoomTreeViewTest::UsersInRoomTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void UsersInRoomTreeViewTest::shouldHaveDefaultValues()
{
    UsersInRoomTreeView w;
    QVERIFY(!w.dragEnabled());
    QVERIFY(!w.rootIsDecorated());
    QVERIFY(w.itemsExpandable());
    QCOMPARE(w.indentation(), 0);
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QVERIFY(w.isHeaderHidden());
}

#include "moc_usersinroomtreeviewtest.cpp"
