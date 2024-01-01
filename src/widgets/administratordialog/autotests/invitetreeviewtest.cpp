/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "invitetreeviewtest.h"
#include "administratordialog/invites/invitetreeview.h"
#include <QTest>
QTEST_MAIN(InviteTreeViewTest)

InviteTreeViewTest::InviteTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void InviteTreeViewTest::shouldHaveDefaultValues()
{
    InviteTreeView w;
    QVERIFY(w.alternatingRowColors());
    QVERIFY(!w.rootIsDecorated());
    QVERIFY(w.isSortingEnabled());
    QCOMPARE(w.selectionMode(), QTreeView::SingleSelection);
    QCOMPARE(w.contextMenuPolicy(), Qt::CustomContextMenu);
    QCOMPARE(w.selectionBehavior(), QAbstractItemView::SelectRows);
}

#include "moc_invitetreeviewtest.cpp"
