/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthtreeviewtest.h"
#include "administratordialog/oauth/oauthtreeview.h"
#include <QTest>
QTEST_MAIN(OauthTreeViewTest)
OauthTreeViewTest::OauthTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void OauthTreeViewTest::shouldHaveDefaultValues()
{
    OauthTreeView treeView(nullptr);
    QVERIFY(treeView.alternatingRowColors());
    QVERIFY(!treeView.rootIsDecorated());
    QVERIFY(treeView.isSortingEnabled());
    QCOMPARE(treeView.contextMenuPolicy(), Qt::CustomContextMenu);
    QCOMPARE(treeView.selectionBehavior(), QAbstractItemView::SelectRows);
    QCOMPARE(treeView.selectionMode(), QAbstractItemView::SingleSelection);
}
