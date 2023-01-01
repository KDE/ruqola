/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showdiscussionswidgettest.h"
#include "discussions/showdiscussionswidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowDiscussionsWidgetTest)
ShowDiscussionsWidgetTest::ShowDiscussionsWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ShowDiscussionsWidgetTest::shouldHaveDefaultValues()
{
    ShowDiscussionsWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchDiscussionLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchDiscussionLineEdit"));
    QVERIFY(mSearchDiscussionLineEdit);
    QVERIFY(mSearchDiscussionLineEdit->text().isEmpty());
    QVERIFY(mSearchDiscussionLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchDiscussionLineEdit->placeholderText().isEmpty());

    auto mDiscussionInfoLabel = w.findChild<QLabel *>(QStringLiteral("mInfo"));
    QVERIFY(mDiscussionInfoLabel);
    QVERIFY(mDiscussionInfoLabel->text().isEmpty());
    QCOMPARE(mDiscussionInfoLabel->textFormat(), Qt::RichText);

    auto mListDiscussions = w.findChild<QListView *>(QStringLiteral("mListDiscussions"));
    QVERIFY(mListDiscussions);
    QVERIFY(mListDiscussions->itemDelegate());
}
