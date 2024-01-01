/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidgettest.h"
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
    auto layout = w.findChild<QVBoxLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(QStringLiteral("mTabWidget"));
    QVERIFY(mTabWidget);

    auto mSearchLineEdit = w.findChild<SearchWithDelayLineEdit *>(QStringLiteral("mSearchLineEdit"));
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());

    auto mRecentUsedFilterProxyModel = w.findChild<EmoticonRecentUsedFilterProxyModel *>(QStringLiteral("mRecentUsedFilterProxyModel"));
    QVERIFY(mRecentUsedFilterProxyModel);
    auto mEmoticonFilterProxyModel = w.findChild<EmoticonModelFilterProxyModel *>(QStringLiteral("mEmoticonFilterProxyModel"));
    QVERIFY(mEmoticonFilterProxyModel);
    auto mEmoticonCustomFilterProxyModel = w.findChild<EmoticonCustomModelFilterProxyModel *>(QStringLiteral("mEmoticonCustomFilterProxyModel"));
    QVERIFY(mEmoticonCustomFilterProxyModel);

    auto mSearchEmojisView = w.findChild<QListView *>(QStringLiteral("mSearchEmojisView"));
    QVERIFY(mSearchEmojisView);
    auto mRecentUsedEmoticonView = w.findChild<RecentUsedEmoticonView *>(QStringLiteral("mRecentUsedEmoticonView"));
    QVERIFY(mRecentUsedEmoticonView);
    auto mCustomEmojiView = w.findChild<QListView *>(QStringLiteral("mCustomEmojiView"));
    QVERIFY(mCustomEmojiView);
}

#include "moc_emoticonmenuwidgettest.cpp"
