/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "texttospeechenqueueinfotest.h"
#include "texttospeech/texttospeechenqueueinfo.h"
#include <QTest>

using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(TextToSpeechEnqueueInfoTest)

TextToSpeechEnqueueInfoTest::TextToSpeechEnqueueInfoTest(QObject *parent)
    : QObject{parent}
{
}

void TextToSpeechEnqueueInfoTest::shouldHaveDefaultValues()
{
    const TextToSpeechEnqueueInfo info;
    QVERIFY(info.accountName().isEmpty());
    QVERIFY(info.messageId().isEmpty());
    QVERIFY(info.roomId().isEmpty());
    QVERIFY(!info.isValid());
}

void TextToSpeechEnqueueInfoTest::shouldBeValidWhenAllValuesAreAssigned()
{
    TextToSpeechEnqueueInfo info;
    info.setAccountName(u"accountname"_s);
    QVERIFY(!info.isValid());
    info.setRoomId("roomid"_ba);
    QVERIFY(!info.isValid());
    info.setMessageId("messageid"_ba);
    QVERIFY(info.isValid());
    QCOMPARE(info.accountName(), u"accountname"_s);
    QCOMPARE(info.roomId(), "roomid");
    QCOMPARE(info.messageId(), "messageid");
}

void TextToSpeechEnqueueInfoTest::shouldCompareInfos()
{
    TextToSpeechEnqueueInfo info;
    info.setAccountName(u"accountname"_s);
    info.setRoomId("roomid"_ba);
    info.setMessageId("messageid"_ba);

    TextToSpeechEnqueueInfo other = info;
    QCOMPARE(info, other);

    other.setMessageId("anotherid"_ba);
    QVERIFY(!(info == other));

    other = info;
    other.setRoomId("anotherroomid"_ba);
    QVERIFY(!(info == other));

    other = info;
    other.setAccountName(u"anotheraccount"_s);
    QVERIFY(!(info == other));
}

#include "moc_texttospeechenqueueinfotest.cpp"
