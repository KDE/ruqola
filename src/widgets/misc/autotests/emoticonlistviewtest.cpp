/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
