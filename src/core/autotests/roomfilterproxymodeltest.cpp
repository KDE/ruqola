/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomfilterproxymodeltest.h"
#include "model/roomfilterproxymodel.h"
#include "model/roommodel.h"
#include <QTest>

QTEST_GUILESS_MAIN(RoomFilterProxyModelTest)

RoomFilterProxyModelTest::RoomFilterProxyModelTest(QObject *parent)
    : QObject(parent)
{
}

void RoomFilterProxyModelTest::shouldHaveDefaultValue()
{
    RoomFilterProxyModel w;
    QCOMPARE(w.sortColumn(), 0);
    QCOMPARE(w.isRecursiveFilteringEnabled(), true);
    RoomModel sourceModel;
    w.setSourceModel(&sourceModel);
    QCOMPARE(w.rowCount(), 0);
    QVERIFY(w.sourceModel());
    QCOMPARE(w.sourceModel(), &sourceModel);
}

#include "moc_roomfilterproxymodeltest.cpp"
