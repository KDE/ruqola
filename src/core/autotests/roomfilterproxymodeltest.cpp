/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomfilterproxymodeltest.h"
#include "model/roomfilterproxymodel.h"
#include "model/roomlistheadingsproxymodel.h"
#include "model/roommodel.h"
#include <QSignalSpy>
#include <QStandardItemModel>
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

void RoomFilterProxyModelTest::shouldEmitRowsInsertedWhenClearingFilter()
{
    // GIVEN a section -> rooms hierarchy
    QStandardItemModel sourceModel;
    auto item = new QStandardItem(QStringLiteral("Room 1"));
    item->setData(QVariant::fromValue(RoomModel::Section::Rooms), RoomModel::RoomSection);
    item->setData(QStringLiteral("Room 1"), RoomModel::RoomName);
    item->setData(true, RoomModel::RoomOpen);
    sourceModel.setItem(0, 0, item);
    RoomListHeadingsProxyModel headings;
    headings.setSourceModel(&sourceModel);
    RoomFilterProxyModel proxy;
    proxy.setSourceModel(&headings);
    QCOMPARE(proxy.rowCount(), 1);

    // WHEN filtering everything out and then clearing the filter again
    proxy.setFilterString(QStringLiteral("zz"));
    QCOMPARE(proxy.rowCount(), 0);
    QSignalSpy insertedSpy(&proxy, &QAbstractItemModel::rowsInserted);
    proxy.setFilterString(QString());

    // THEN the section comes back with a rowsInserted signal, so that views can react to it
    QCOMPARE(proxy.rowCount(), 1);
    const QModelIndex sectionIndex = proxy.index(0, 0);
    QCOMPARE(proxy.rowCount(sectionIndex), 1);
    QVERIFY(!insertedSpy.isEmpty());
    QCOMPARE(insertedSpy.constFirst().at(0).value<QModelIndex>(), QModelIndex());
}

#include "moc_roomfilterproxymodeltest.cpp"
