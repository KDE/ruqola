/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messageencryptedtest.h"
#include "messages/messageencrypted.h"
#include <QTest>
QTEST_GUILESS_MAIN(MessageEncryptedTest)

MessageEncryptedTest::MessageEncryptedTest(QObject *parent)
    : QObject{parent}
{
}

void MessageEncryptedTest::shouldHaveDefaultValues()
{
    const MessageEncrypted w;
    QVERIFY(w.algorithm().isEmpty());
    QVERIFY(w.ciphertext().isEmpty());
    QVERIFY(w.keyId().isEmpty());
    QVERIFY(w.iv().isEmpty());
    QVERIFY(!w.isValid());
}
