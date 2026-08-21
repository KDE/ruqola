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
    TextToSpeechEnqueueInfo info;
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
    info.setRoomId("roomid");
    QVERIFY(!info.isValid());
    info.setMessageId("messageid");
    QVERIFY(info.isValid());
    QCOMPARE(info.accountName(), u"accountname"_s);
    QCOMPARE(info.roomId(), "roomid");
    QCOMPARE(info.messageId(), "messageid");
}

void TextToSpeechEnqueueInfoTest::shouldCompareInfos()
{
    TextToSpeechEnqueueInfo info;
    info.setAccountName(u"accountname"_s);
    info.setRoomId("roomid");
    info.setMessageId("messageid");

    TextToSpeechEnqueueInfo other = info;
    QCOMPARE(info, other);

    other.setMessageId("anotherid");
    QVERIFY(!(info == other));

    other = info;
    other.setRoomId("anotherroomid");
    QVERIFY(!(info == other));

    other = info;
    other.setAccountName(u"anotheraccount"_s);
    QVERIFY(!(info == other));
}

#include "moc_texttospeechenqueueinfotest.cpp"
