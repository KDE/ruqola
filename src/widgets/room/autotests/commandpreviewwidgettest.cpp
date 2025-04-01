/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "commandpreviewwidgettest.h"
#include "room/commandpreviewwidget.h"
#include <QHBoxLayout>
#include <QListView>
#include <QStackedWidget>
#include <QTest>
QTEST_MAIN(CommandPreviewWidgetTest)

CommandPreviewWidgetTest::CommandPreviewWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void CommandPreviewWidgetTest::shouldHaveDefaultValues()
{
    CommandPreviewWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mListView = w.findChild<QListView *>(QStringLiteral("mListView"));
    QVERIFY(mListView);
    QCOMPARE(mListView->viewMode(), QListView::IconMode);
    QCOMPARE(mListView->flow(), QListView::LeftToRight);
    QCOMPARE(mListView->resizeMode(), QListView::Adjust);
    QVERIFY(!mListView->wordWrap());
    QCOMPARE(mListView->spacing(), 2);

    auto mStackWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackWidget"));
    QVERIFY(mStackWidget);
}

#include "moc_commandpreviewwidgettest.cpp"
