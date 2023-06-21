/*
  SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonlistviewtest.h"
#include "emoticonlistview.h"
#include <QTest>
QTEST_MAIN(EmoticonListViewTest)
EmoticonListViewTest::EmoticonListViewTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonListViewTest::shouldHaveDefaultValues()
{
    TextEmoticonsWidgets::EmoticonListView w;
    QCOMPARE(w.viewMode(), QListView::IconMode);
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    // QVERIFY(w.uniformItemSizes());
    QVERIFY(!w.dragEnabled());
    QVERIFY(w.hasMouseTracking());
    QCOMPARE(w.contextMenuPolicy(), Qt::DefaultContextMenu);
    QCOMPARE(w.itemDelegate()->objectName(), QStringLiteral("emoticonDelegate"));
}

#include "moc_emoticonlistviewtest.cpp"
