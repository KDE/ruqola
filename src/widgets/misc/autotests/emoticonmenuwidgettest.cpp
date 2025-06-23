/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/emoticonmenuwidget.h"
#include "misc/emoticonrecentusedfilterproxymodel.h"
#include "misc/recentusedemoticonview.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/emoticoncustommodelfilterproxymodel.h"
#include "model/emoticonmodelfilterproxymodel.h"
#include <QStandardPaths>
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(EmoticonMenuWidgetTest)
EmoticonMenuWidgetTest::EmoticonMenuWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void EmoticonMenuWidgetTest::shouldHaveDefaultValues()
{
    EmoticonMenuWidget w;
    auto layout = w.findChild<QVBoxLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(u"mTabWidget"_s);
    QVERIFY(mTabWidget);

    auto mSearchLineEdit = w.findChild<SearchWithDelayLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mRecentUsedFilterProxyModel = w.findChild<EmoticonRecentUsedFilterProxyModel *>(u"mRecentUsedFilterProxyModel"_s);
    QVERIFY(mRecentUsedFilterProxyModel);
    auto mEmoticonFilterProxyModel = w.findChild<EmoticonModelFilterProxyModel *>(u"mEmoticonFilterProxyModel"_s);
    QVERIFY(mEmoticonFilterProxyModel);
    auto mEmoticonCustomFilterProxyModel = w.findChild<EmoticonCustomModelFilterProxyModel *>(u"mEmoticonCustomFilterProxyModel"_s);
    QVERIFY(mEmoticonCustomFilterProxyModel);

    auto mSearchEmojisView = w.findChild<QListView *>(u"mSearchEmojisView"_s);
    QVERIFY(mSearchEmojisView);
    auto mRecentUsedEmoticonView = w.findChild<RecentUsedEmoticonView *>(u"mRecentUsedEmoticonView"_s);
    QVERIFY(mRecentUsedEmoticonView);
    auto mCustomEmojiView = w.findChild<QListView *>(u"mCustomEmojiView"_s);
    QVERIFY(mCustomEmojiView);
}

#include "moc_emoticonmenuwidgettest.cpp"
