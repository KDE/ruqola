/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchchannelwidgettest.h"
#include "dialogs/searchchannelwidget.h"
#include "misc/searchwithdelaylineedit.h"
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SearchChannelWidgetTest)
SearchChannelWidgetTest::SearchChannelWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SearchChannelWidgetTest::shouldHaveDefaultValues()
{
    SearchChannelWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchLineEdit = w.findChild<SearchWithDelayLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mResultListWidget = w.findChild<QListView *>(QStringLiteral("mResultListWidget"));
    QVERIFY(mResultListWidget);
}
