/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "texttospeechenqueuemanagertest.h"
#include "texttospeech/texttospeechenqueuemanager.h"
#include <QTest>
QTEST_GUILESS_MAIN(TextToSpeechEnqueueManagerTest)

using namespace Qt::Literals::StringLiterals;
namespace
{
[[nodiscard]] TextToSpeechEnqueueInfo createInfo(const QByteArray &messageId)
{
    TextToSpeechEnqueueInfo info;
    info.setAccountName(u"accountname"_s);
    info.setRoomId("roomid");
    info.setMessageId(messageId);
    return info;
}
}

TextToSpeechEnqueueManagerTest::TextToSpeechEnqueueManagerTest(QObject *parent)
    : QObject{parent}
{
}

void TextToSpeechEnqueueManagerTest::shouldHaveDefaultValues()
{
    const TextToSpeechEnqueueManager manager;
    QVERIFY(manager.enqueueList().isEmpty());
    QVERIFY(!manager.value(0).isValid());
    QVERIFY(!manager.value(-1).isValid());
}

void TextToSpeechEnqueueManagerTest::shouldStoreInfoAtEnqueuePosition()
{
    TextToSpeechEnqueueManager manager;
    const auto first = createInfo("messageid1");
    const auto second = createInfo("messageid2");

    // The synthesizer identifies each utterance by its position in the queue.
    manager.insert(first);
    // No information about the enqueued text => an invalid info keeps positions in sync.
    manager.insert({});
    manager.insert(second);

    QCOMPARE(manager.enqueueList().count(), 3);
    QCOMPARE(manager.value(0), first);
    QVERIFY(!manager.value(1).isValid());
    QCOMPARE(manager.value(2), second);
    QVERIFY(manager.contains(first));
    QVERIFY(manager.contains(second));
    QVERIFY(!manager.contains(createInfo("messageid3")));
}

void TextToSpeechEnqueueManagerTest::shouldNotContainsInvalidInfo()
{
    TextToSpeechEnqueueManager manager;
    manager.insert({});
    // We can enqueue several texts without info: they must not deduplicate each other.
    QVERIFY(!manager.contains({}));
}

void TextToSpeechEnqueueManagerTest::shouldClearList()
{
    TextToSpeechEnqueueManager manager;
    manager.insert(createInfo("messageid1"));
    manager.clear();
    QVERIFY(manager.enqueueList().isEmpty());
}

void TextToSpeechEnqueueManagerTest::shouldRemoveLastInfo()
{
    TextToSpeechEnqueueManager manager;
    manager.removeLast();
    QVERIFY(manager.enqueueList().isEmpty());

    const auto first = createInfo("messageid1");
    manager.insert(first);
    manager.insert(createInfo("messageid2"));
    manager.removeLast();
    QCOMPARE(manager.enqueueList().count(), 1);
    QCOMPARE(manager.value(0), first);
}

#include "moc_texttospeechenqueuemanagertest.cpp"
