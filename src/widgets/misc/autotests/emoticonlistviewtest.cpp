/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "emoticonlistviewtest.h"
#include "misc/emoticonlistview.h"
#include <QTest>
QTEST_MAIN(EmoticonListViewTest)

EmoticonListViewTest::EmoticonListViewTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonListViewTest::shouldHaveDefaultValues()
{
    EmoticonListView w;
    QCOMPARE(w.viewMode(), QListView::IconMode);
    QCOMPARE(w.horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
}
