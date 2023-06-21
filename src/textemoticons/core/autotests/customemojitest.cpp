/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customemojitest.h"
#include "customemoji.h"
#include "emoticonunicodeutils.h"
#include <QTest>
QTEST_GUILESS_MAIN(CustomEmojiTest)

CustomEmojiTest::CustomEmojiTest(QObject *parent)
    : QObject{parent}
{
}

void CustomEmojiTest::shouldHaveDefaultValues()
{
    TextEmoticonsCore::CustomEmoji w;
    QVERIFY(!w.category().isEmpty());
    QVERIFY(w.identifier().isEmpty());
    QCOMPARE(w.category(), TextEmoticonsCore::EmoticonUnicodeUtils::customIdentifier());
    QVERIFY(!w.isAnimatedEmoji());
}

#include "moc_customemojitest.cpp"
