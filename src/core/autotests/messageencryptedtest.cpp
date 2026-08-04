/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messageencryptedtest.h"
#include "config-ruqola.h"
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

void MessageEncryptedTest::shouldDecryptV2Payload()
{
    // Test vector generated with AES-256-GCM, key='k'*32, iv="0123456789ab", plaintext={"msg":"hello e2e"}
    const QByteArray sessionKey(32, 'k');
    const QByteArray iv("0123456789ab");
    const QByteArray plainText("{\"msg\":\"hello e2e\"}");
    const QByteArray encryptedPayload = QByteArray::fromBase64("ej0KsqEKP7tIhPFauxLZfLCDiI6PY2Ex68Kv4kt2sCFIA24=");
    QVERIFY(!encryptedPayload.isEmpty());

    MessageEncrypted encrypted;
    encrypted.setAlgorithm("rc.v2.aes-sha2");
    encrypted.setKeyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");
    encrypted.setIv(iv.toBase64());
    encrypted.setCiphertext(QString::fromLatin1(encryptedPayload.toBase64()));

#if USE_E2E_SUPPORT
    QCOMPARE(encrypted.decrypt(sessionKey), plainText);

    const QByteArray wrongSessionKey(32, 'x');
    QVERIFY(encrypted.decrypt(wrongSessionKey).isEmpty());
#else
    QVERIFY(encrypted.decrypt(sessionKey).isEmpty());
#endif
}

#include "moc_messageencryptedtest.cpp"
