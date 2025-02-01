/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "searchmessagelinewidgettest.h"
#include "dialogs/searchmessagelinewidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(SearchMessageLineWidgetTest)

SearchMessageLineWidgetTest::SearchMessageLineWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void SearchMessageLineWidgetTest::shouldHaveDefaultValues()
{
    SearchMessageLineWidget w;

    auto mMainLayout = w.findChild<QHBoxLayout *>(QStringLiteral("mMainLayout"));
    QVERIFY(mMainLayout);
    QCOMPARE(mMainLayout->contentsMargins(), QMargins{});

    auto mSearchMessageLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchMessageLineEdit"));
    QVERIFY(mSearchMessageLineEdit);
    QVERIFY(mSearchMessageLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchMessageLineEdit->placeholderText().isEmpty());
}

#include "moc_searchmessagelinewidgettest.cpp"
