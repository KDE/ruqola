/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "emojiwidgetactiontest.h"
#include "room/emojiwidgetaction.h"
#include <QTest>
#include <QToolButton>
#include <qboxlayout.h>
QTEST_MAIN(EmojiWidgetActionTest)
using namespace Qt::Literals::StringLiterals;
EmojiWidgetActionTest::EmojiWidgetActionTest(QObject *parent)
    : QObject{parent}
{
}

void EmojiWidgetActionTest::shouldHaveDefaultValuesEmojiWidgetActionWidget()
{
    const EmojiWidgetActionWidget d({});
    auto mainLayout = d.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto selectMoreEmojiButton = d.findChild<QToolButton *>(u"selectMoreEmojiButton"_s);
    QVERIFY(selectMoreEmojiButton);
    QVERIFY(selectMoreEmojiButton->autoRaise());
}

#include "moc_emojiwidgetactiontest.cpp"
