/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mSearchLineEdit = w.findChild<SearchMessageWithDelayLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mResultListWidget = w.findChild<MessageListView *>(u"mResultListWidget"_s);
    QVERIFY(mResultListWidget);
    QCOMPARE(mResultListWidget->mode(), MessageListView::Mode::Viewing);

    auto mSearchLabel = w.findChild<QLabel *>(u"mSearchLabel"_s);
    QVERIFY(mSearchLabel);
    QVERIFY(mSearchLabel->text().isEmpty());

    auto labelRegularExpression = w.findChild<QLabel *>(u"labelRegularExpression"_s);
    QVERIFY(labelRegularExpression);
    QVERIFY(!labelRegularExpression->text().isEmpty());
    QVERIFY(labelRegularExpression->openExternalLinks());
}

#include "moc_searchmessagewidgettest.cpp"
