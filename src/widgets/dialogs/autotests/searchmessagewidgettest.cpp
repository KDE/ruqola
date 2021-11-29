/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchmessagewidgettest.h"
#include "dialogs/searchmessagewidget.h"
#include "misc/searchwithdelaylineedit.h"
#include "room/messagelistview.h"
#include <QLabel>
#include <QTest>
QTEST_MAIN(SearchMessageWidgetTest)

SearchMessageWidgetTest::SearchMessageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void SearchMessageWidgetTest::shouldHaveDefaultValues()
{
    SearchMessageWidget w;

    auto mSearchLineEdit = w.findChild<SearchWithDelayLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mResultListWidget = w.findChild<MessageListView *>(QStringLiteral("mResultListWidget"));
    QVERIFY(mResultListWidget);
    QCOMPARE(mResultListWidget->mode(), MessageListView::Mode::Viewing);

    auto mSearchLabel = w.findChild<QLabel *>(QStringLiteral("mSearchLabel"));
    QVERIFY(mSearchLabel);
    QVERIFY(mSearchLabel->text().isEmpty());
}
