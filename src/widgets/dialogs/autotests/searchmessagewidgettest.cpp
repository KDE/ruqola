/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagewidgettest.h"
#include "dialogs/searchmessagewidget.h"
#include "dialogs/searchmessagewithdelaylineedit.h"
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
    SearchMessageWidget w(nullptr);

    auto mSearchLineEdit = w.findChild<SearchMessageWithDelayLineEdit *>(QStringLiteral("mSearchLineEdit"));
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

#include "moc_searchmessagewidgettest.cpp"
