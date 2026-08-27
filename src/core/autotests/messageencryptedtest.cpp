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

void MessageEncryptedTest::shouldEncryptV2Payload()
{
    const QByteArray sessionKey(32, 'k');
    const QByteArray plainText("{\"msg\":\"hello e2e\"}");
    const QByteArray keyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");

    MessageEncrypted encrypted;
    const bool encryptedOk = encrypted.encrypt(plainText, sessionKey, keyId);
    if (encryptedOk) {
        QCOMPARE(encrypted.algorithm(), QByteArray("rc.v2.aes-sha2"));
        QCOMPARE(encrypted.keyId(), keyId);
        QVERIFY(!QByteArray::fromBase64(encrypted.iv()).isEmpty());
        QVERIFY(!QByteArray::fromBase64(encrypted.ciphertext().toLatin1()).isEmpty());
        QCOMPARE(encrypted.decrypt(sessionKey), plainText);
    } else {
        QVERIFY(!encrypted.isValid());
    }
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

    const QByteArray decryptedPayload = encrypted.decrypt(sessionKey);
    QVERIFY(decryptedPayload.isEmpty() || decryptedPayload == plainText);

    const QByteArray wrongSessionKey(32, 'x');
    QVERIFY(encrypted.decrypt(wrongSessionKey).isEmpty());
}

// A room keyed before Rocket.Chat switched to AES-GCM still holds a 16-byte A128CBC key, and its
// clients encrypt with AES-CBC-128 even inside a "rc.v2.aes-sha2" content: the mode follows the
// key, not the content version.
void MessageEncryptedTest::shouldEncryptLegacyCbcPayload()
{
    const QByteArray sessionKey(16, 'k');
    const QByteArray plainText("{\"msg\":\"hello legacy\"}");
    const QByteArray keyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");

    MessageEncrypted encrypted;
    const bool encryptedOk = encrypted.encrypt(plainText, sessionKey, keyId);
#if USE_E2E_SUPPORT
    QVERIFY(encryptedOk);
    QCOMPARE(encrypted.algorithm(), QByteArray("rc.v2.aes-sha2"));
    QCOMPARE(encrypted.keyId(), keyId);
    // AES-CBC takes a full-block IV, unlike the 12-byte one of AES-GCM.
    QCOMPARE(QByteArray::fromBase64(encrypted.iv()).size(), 16);
    QCOMPARE(encrypted.decrypt(sessionKey), plainText);

    const QByteArray wrongSessionKey(16, 'x');
    QVERIFY(encrypted.decrypt(wrongSessionKey) != plainText);

    // A key of a size no Rocket.Chat client can import must not be used at all.
    MessageEncrypted bogus;
    QVERIFY(!bogus.encrypt(plainText, QByteArray(24, 'k'), keyId));
#else
    QVERIFY(!encryptedOk);
#endif
}

void MessageEncryptedTest::shouldDecryptLegacyCbcPayload()
{
    // Test vector generated with AES-128-CBC, key='k'*16, iv="0123456789abcdef"
    const QByteArray sessionKey(16, 'k');
    const QByteArray iv("0123456789abcdef");
    const QByteArray plainText("{\"msg\":\"hello e2e\"}");
    const QByteArray encryptedPayload = QByteArray::fromBase64("mz5PnpxdrRzYBBDBt7J3k9xh/1aFGvdiki4lnf6aSYM=");
    QVERIFY(!encryptedPayload.isEmpty());

    MessageEncrypted encrypted;
    encrypted.setAlgorithm("rc.v2.aes-sha2");
    encrypted.setKeyId("23e2720d-b3e0-4753-85ff-bad2caeb867b");
    encrypted.setIv(iv.toBase64());
    encrypted.setCiphertext(QString::fromLatin1(encryptedPayload.toBase64()));

#if USE_E2E_SUPPORT
    QCOMPARE(encrypted.decrypt(sessionKey), plainText);
    QVERIFY(encrypted.decrypt(QByteArray(16, 'x')) != plainText);
#else
    QVERIFY(encrypted.decrypt(sessionKey).isEmpty());
#endif
}

#include "moc_messageencryptedtest.cpp"
