/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechutiltest.h"
#include "texttospeechutil.h"
#include <QTest>
QTEST_GUILESS_MAIN(TextToSpeechUtilTest)
TextToSpeechUtilTest::TextToSpeechUtilTest(QObject *parent)
    : QObject{parent}
{
}

void TextToSpeechUtilTest::shouldHaveDefaultValues()
{
    QCOMPARE(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName(), QStringLiteral("texttospeechrc"));
    QCOMPARE(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName(), QStringLiteral("Settings"));
}
