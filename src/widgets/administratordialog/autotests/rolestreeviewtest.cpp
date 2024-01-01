/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rolestreeviewtest.h"
#include "administratordialog/roles/rolestreeview.h"
#include <QHeaderView>
#include <QTest>
QTEST_MAIN(RolesTreeViewTest)
RolesTreeViewTest::RolesTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void RolesTreeViewTest::shouldHaveDefaultValues()
{
    RolesTreeView w;
    QVERIFY(!w.rootIsDecorated());
    QVERIFY(w.isSortingEnabled());
    QCOMPARE(w.contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(w.header()->sectionsClickable());
}

#include "moc_rolestreeviewtest.cpp"
