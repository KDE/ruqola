/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionstreeviewtest.h"
#include "administratordialog/permissions/permissionstreeview.h"
#include <QHeaderView>
#include <QTest>
QTEST_MAIN(PermissionsTreeViewTest)
PermissionsTreeViewTest::PermissionsTreeViewTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionsTreeViewTest::shouldHaveDefaultValues()
{
    PermissionsTreeView w;
    QVERIFY(!w.rootIsDecorated());
    QVERIFY(w.isSortingEnabled());
    QCOMPARE(w.contextMenuPolicy(), Qt::CustomContextMenu);
    QVERIFY(w.header()->sectionsClickable());
}

#include "moc_permissionstreeviewtest.cpp"
