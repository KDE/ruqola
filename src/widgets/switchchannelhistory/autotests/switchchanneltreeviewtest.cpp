/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "switchchanneltreeviewtest.h"
#include "switchchannelhistory/switchchanneltreeview.h"
#include <QTest>
QTEST_MAIN(SwitchChannelTreeViewTest)
SwitchChannelTreeViewTest::SwitchChannelTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void SwitchChannelTreeViewTest::shouldHaveDefaultValues()
{
    SwitchChannelTreeView d;
    QVERIFY(d.isHeaderHidden());
    QVERIFY(!d.rootIsDecorated());
    QCOMPARE(d.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(d.textElideMode(), Qt::ElideMiddle);
    QCOMPARE(d.selectionMode(), QAbstractItemView::SingleSelection);
    QCOMPARE(d.selectionBehavior(), QAbstractItemView::SelectRows);
    QCOMPARE(d.windowFlags(), Qt::Popup | Qt::FramelessWindowHint);
}

#include "moc_switchchanneltreeviewtest.cpp"
