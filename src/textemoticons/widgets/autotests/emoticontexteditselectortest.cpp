/*
  SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticontexteditselectortest.h"
#include "emoticoncategorybuttons.h"
#include "emoticonlistview.h"
#include "emoticontexteditselector.h"
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(EmoticonTextEditSelectorTest)

EmoticonTextEditSelectorTest::EmoticonTextEditSelectorTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonTextEditSelectorTest::shouldHaveDefaultValues()
{
    TextEmoticonsWidgets::EmoticonTextEditSelector w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mCategoryButtons = w.findChild<TextEmoticonsWidgets::EmoticonCategoryButtons *>(QStringLiteral("mCategoryButtons"));
    QVERIFY(mCategoryButtons);

    auto mEmoticonListView = w.findChild<TextEmoticonsWidgets::EmoticonListView *>(QStringLiteral("mEmoticonListView"));
    QVERIFY(mEmoticonListView);

    auto mSearchUnicodeLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mSearchUnicodeLineEdit"));
    QVERIFY(mSearchUnicodeLineEdit);
    QVERIFY(mSearchUnicodeLineEdit->isClearButtonEnabled());
    QVERIFY(mSearchUnicodeLineEdit->text().isEmpty());
    QVERIFY(!mSearchUnicodeLineEdit->placeholderText().isEmpty());

    QVERIFY(!w.customEmojiSupport());
}

#include "moc_emoticontexteditselectortest.cpp"
