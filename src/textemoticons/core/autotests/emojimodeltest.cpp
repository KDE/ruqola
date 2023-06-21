/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojimodeltest.h"
#include "emojimodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(EmojiModelTest)

EmojiModelTest::EmojiModelTest(QObject *parent)
    : QObject{parent}
{
}

void EmojiModelTest::shouldHaveDefaultValues()
{
    TextEmoticonsCore::EmojiModel w;
    QVERIFY(w.emoticonList().isEmpty());
    QVERIFY(w.customEmojiList().isEmpty());
    QVERIFY(!w.customEmojiIconManager());
}

#include "moc_emojimodeltest.cpp"
