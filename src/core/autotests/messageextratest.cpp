/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageextratest.h"
#include "messages/messageextra.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageExtraTest)
MessageExtraTest::MessageExtraTest(QObject *parent)
    : QObject{parent}
{
}

void MessageExtraTest::shouldHaveDefaultValues()
{
    MessageExtra extra;
    QVERIFY(extra.localTranslation().isEmpty());
    QVERIFY(extra.discussionRoomId().isEmpty());
    QVERIFY(extra.threadMessageId().isEmpty());
    QCOMPARE(extra.threadCount(), 0);
    QCOMPARE(extra.discussionCount(), 0);
}

#include "moc_messageextratest.cpp"
