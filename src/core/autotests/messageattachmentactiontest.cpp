/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messageattachmentactiontest.h"
#include "messages/messageattachmentaction.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageAttachmentActionTest)

MessageAttachmentActionTest::MessageAttachmentActionTest(QObject *parent)
    : QObject{parent}
{
}

void MessageAttachmentActionTest::shouldHaveDefaultValues()
{
    const MessageAttachmentAction act;
    QVERIFY(act.msg().isEmpty());
    QVERIFY(act.text().isEmpty());
    QVERIFY(act.type().isEmpty());
}

#include "moc_messageattachmentactiontest.cpp"
