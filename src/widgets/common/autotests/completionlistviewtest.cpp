/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "completionlistviewtest.h"
#include "common/completionlistview.h"
#include <QTest>
QTEST_MAIN(CompletionListViewTest)
CompletionListViewTest::CompletionListViewTest(QObject *parent)
    : QObject{parent}
{
}

void CompletionListViewTest::shouldHaveDefaultValues()
{
    CompletionListView w;
    QVERIFY(w.uniformItemSizes());
    QCOMPARE(w.windowType(), Qt::Popup);
}

#include "moc_completionlistviewtest.cpp"
