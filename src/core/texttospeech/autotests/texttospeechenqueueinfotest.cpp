/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "texttospeechenqueueinfotest.h"
#include "texttospeech/texttospeechenqueueinfo.h"
#include <QTest>
QTEST_GUILESS_MAIN(TextToSpeechEnqueueInfoTest)

TextToSpeechEnqueueInfoTest::TextToSpeechEnqueueInfoTest(QObject *parent)
    : QObject{parent}
{
}

void TextToSpeechEnqueueInfoTest::shouldHaveDefaultValues()
{
    TextToSpeechEnqueueInfo info;
    QVERIFY(info.accountName().isEmpty());
    QVERIFY(info.messageId().isEmpty());
    QVERIFY(info.roomId().isEmpty());
    QVERIFY(!info.isValid());
}
