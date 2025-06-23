/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showdiscussionswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchDiscussionLineEdit = w.findChild<QLineEdit *>(u"mSearchDiscussionLineEdit"_s);
    QVERIFY(mSearchDiscussionLineEdit);
    QVERIFY(mSearchDiscussionLineEdit->text().isEmpty());
    QVERIFY(mSearchDiscussionLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchDiscussionLineEdit->placeholderText().isEmpty());

    auto mDiscussionInfoLabel = w.findChild<QLabel *>(u"mInfo"_s);
    QVERIFY(mDiscussionInfoLabel);
    QVERIFY(mDiscussionInfoLabel->text().isEmpty());
    QCOMPARE(mDiscussionInfoLabel->textFormat(), Qt::RichText);

    auto mListDiscussions = w.findChild<QListView *>(u"mListDiscussions"_s);
    QVERIFY(mListDiscussions);
    QVERIFY(mListDiscussions->itemDelegate());
}

#include "moc_showdiscussionswidgettest.cpp"
