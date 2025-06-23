/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channellistviewtest.h"
using namespace Qt::Literals::StringLiterals;

#include "channellist/channellistview.h"
#include "model/roomfilterproxymodel.h"
#include <QTest>

QTEST_MAIN(ChannelListViewTest)
ChannelListViewTest::ChannelListViewTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelListViewTest::shouldHaveDefaultValues()
{
    ChannelListView w;
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(w.itemDelegate()->objectName(), u"mChannelListDelegate"_s);
    QCOMPARE(w.isHeaderHidden(), true);
    QCOMPARE(w.rootIsDecorated(), false);
    QCOMPARE(w.uniformRowHeights(), false);
    QCOMPARE(w.itemsExpandable(), false);
    QCOMPARE(w.indentation(), 0);
    QVERIFY(w.filterModel());
    QCOMPARE(w.filterModel()->objectName(), u"mRoomFilterProxyModel"_s);
}

#include "moc_channellistviewtest.cpp"
