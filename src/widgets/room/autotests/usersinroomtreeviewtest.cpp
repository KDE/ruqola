/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomtreeviewtest.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroomlistheadingsproxymodel.h"
#include "model/usersforroommodel.h"
#include "room/usersinroomtreeview.h"
#include <QTest>
QTEST_MAIN(UsersInRoomTreeViewTest)

UsersInRoomTreeViewTest::UsersInRoomTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void UsersInRoomTreeViewTest::shouldHaveDefaultValues()
{
    const UsersInRoomTreeView w;
    QVERIFY(!w.dragEnabled());
    QVERIFY(!w.rootIsDecorated());
    QVERIFY(w.itemsExpandable());
    QVERIFY(!w.expandsOnDoubleClick());
    QCOMPARE(w.indentation(), 0);
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QVERIFY(w.isHeaderHidden());
}

void UsersInRoomTreeViewTest::shouldKeepSectionProxyChain()
{
    const UsersInRoomTreeView w;
    auto *const filterProxy = w.usersForRoomFilterProxy();
    QVERIFY(filterProxy);

    auto *const headingsProxy = qobject_cast<UsersForRoomListHeadingsProxyModel *>(filterProxy->sourceModel());
    QVERIFY(headingsProxy);

    UsersForRoomModel usersModel;
    headingsProxy->setSourceModel(&usersModel);

    QCOMPARE(headingsProxy->sourceModel(), &usersModel);
    QVERIFY(filterProxy->sourceModel() != &usersModel);
}

#include "moc_usersinroomtreeviewtest.cpp"
